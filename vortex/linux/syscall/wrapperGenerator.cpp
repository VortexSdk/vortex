#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Helper function to replace the last occurrence of a substring
std::string
replaceLast(const std::string& str, const std::string& what, const std::string& replacement) {
    std::size_t pos = str.rfind(what);
    if (pos == std::string::npos) return str;
    return str.substr(0, pos) + replacement + str.substr(pos + what.length());
}

// Structure to store argument data
struct Argument {
    std::string name;
    std::string type;
};

// Structure to store function definition
struct Definition {
    std::string line;
    std::string name;
    std::vector<Argument> args;
};

// Function to parse function definitions from the data
std::vector<Definition> parseDefinitions(const std::string& data) {
    std::vector<Definition> definitions;
    std::istringstream stream(data);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.length() < 10 || (line [0] == '/' && line [1] == '/')) continue;

        std::size_t open_brac_idx  = line.find('(');
        std::size_t close_brac_idx = line.find(')');
        if (open_brac_idx == std::string::npos || close_brac_idx == std::string::npos) continue;

        std::string name     = line.substr(5, open_brac_idx - 5);
        std::string args_str = line.substr(open_brac_idx + 1, close_brac_idx - open_brac_idx - 1);

        std::vector<Argument> args;

        for (const auto& arg : std::views::split(args_str, ',')) {
            std::string arg_str = std::string(arg.begin(), arg.end());
            arg_str = std::regex_replace(arg_str, std::regex("^\\s+"), ""); // Trim leading spaces

            std::vector<std::string> words;
            std::istringstream argStream(arg_str);
            std::string word;
            while (argStream >> word) words.push_back(word);

            if (words.empty()) continue;

            std::string arg_name = replaceLast(replaceLast(words.back(), "*", ""), "*", "");
            arg_name             = !arg_name.empty() ? arg_name : "";

            if (arg_name.compare("interval")) arg_name = replaceLast(arg_name, "int", "");
            arg_name             = replaceLast(arg_name, "long", "");
            arg_name             = replaceLast(arg_name, "short", "");

            std::string arg_type = replaceLast(arg_str, arg_name, "");
            arg_type             = !arg_type.empty() ? arg_type : arg_name;
            arg_type             = std::regex_replace(arg_type, std::regex("\\s+$"), "");

            if (arg_type == arg_name) {
                arg_name = "";
            }

            args.push_back({arg_name, arg_type}); // Trim trailing spaces
        }

        definitions.push_back({line, name, args});
    }

    return definitions;
}

std::vector<std::string> alphabet = {"a", "b", "c", "d", "e", "f", "g", "h", "i",
                                     "j", "k", "l", "m", "n", "o", "p", "q", "r",
                                     "s", "t", "u", "v", "w", "x", "y", "z"};
std::unordered_map<std::string, std::string> file_descriptor_map = {
    {"unsigned int",  "FdU" },
    {"int",           "FdI" },
    {"unsigned long", "FdUL"},
    {"long",          "FdL" },
};
std::unordered_map<std::string, std::string> syscall_name_map = {
    {"newuname",         "uname"          },
    {"statfs64",         "statfs"         },
    {"fstatfs64",        "fstatfs"        },
    {"sendfile64",       "sendfile"       },
    {"fadvise64_64",     "fadvise64"      },
    {"sync_file_range2", "sync_file_range"},
};
std::unordered_map<std::string, std::string> syscall_arg_type_map = {
    {"size_t",                      "usize"               },
    {"ssize_t",                     "isize"               },
    {"uint32_t",                    "u32"                 },
    {"size_t *",                    "usize *"             },
    {"struct statx *",              "statx_t *"           },
    {"struct mmsghdr *",            "mmsghdr *"           },
    {"struct sockaddr *",           "sockaddr *"          },
    {"struct linux_dirent64 *",     "linux_dirent64 *"    },
    {"struct sched_param *",        "sched_param *"       },
    {"struct getcpu_cache *",       "getcpu_cache *"      },
    {"struct user_msghdr *",        "user_msghdr *"       },
    {"struct file_handle *",        "file_handle *"       },
    {"struct __kernel_timespec *",  "timespec *"          },
    {"const struct __aio_sigset *", "const __aio_sigset *"},
};
std::unordered_map<std::string, std::string> syscall_ret_type_map = {
    {"mmap",    "u8*"  },
    {"brk",     "u8*"  },
    {"mremap",  "u8*"  },
    {"sbrk",    "u8*"  },
    {"shmat",   "void*"},
    {"getcwd",  "char*"},
    {"getpgrp", "pid_t"},
    {"getpid",  "pid_t"},
    {"getppid", "pid_t"},
    {"gettid",  "pid_t"},
    {"fork",    "pid_t"},
    {"vfork",   "pid_t"},
    {"waitpid", "pid_t"},
    {"waitid",  "pid_t"},
    {"wait4",   "pid_t"},
    {"clone",   "pid_t"},
    {"lseek",   "off_t"},
    {"getpgid", "pid_t"},
    {"getsid",  "pid_t"},
};
std::string createSyscallWrapper(const std::vector<Definition>& definitions) {
    std::string s = "// Warning: This file is auto generated. Don't edit this file directly!\n\n"
                    "#pragma once\n"
                    "#include \"syscall_impl.hpp\"\n\n"
                    "#include \"wrapperHelper.hpp\"\n\n"
                    "// NOLINTBEGIN\n\n";

    for (const auto& d : definitions) {
        auto sys_name = d.name;
        if (syscall_name_map.contains(sys_name)) sys_name = syscall_name_map [sys_name];

        std::string ret = "usize";
        if (syscall_ret_type_map.contains(sys_name)) ret = syscall_ret_type_map [sys_name];

        s += "static SysRes<" + ret + "> " + sys_name + "(";
        const bool has_args = d.args.size() > 0 && d.args [0].type.compare("void");

        if (has_args) {
            for (const auto& [i, a] : d.args | std::views::enumerate) {
                auto t    = a.type;
                auto name = !a.name.empty() ? a.name : alphabet [i];
                if (syscall_arg_type_map.contains(t)) t = syscall_arg_type_map [t];

                if (a.name == "fd" || a.name == "fdin" || a.name == "fd_in" || a.name == "fdout" ||
                    a.name == "fd_out" || std::regex_search(a.name, std::regex(".*fd"))) {
                    if (a.name != "nfds" && file_descriptor_map.contains(t))
                        t = file_descriptor_map [t];
                } else if (a.name == "addr" && t == "unsigned long") t = "void *";

                s += t + " " + name + ",";
            }
            s.erase(s.end() - 1);
        }

        if (sys_name == "umount") sys_name = "umount2";
        if (has_args) {
            s += ") { return syscall(__NR_" + sys_name + ",";
            for (const auto& [i, a] : d.args | std::views::enumerate) {
                auto name = !a.name.empty() ? a.name : alphabet [i];
                s += name + ",";
            }
            s.erase(s.end() - 1);
        } else {
            s += ") { return syscall(__NR_" + sys_name;
        }

        if (ret == "usize") s += ");}\n";
        else s += ").unsafe_cast<" + ret + ">();}\n";
    }

    s += "\n// NOLINTEND\n";

    return s;
}

// Function to print definitions and arguments
void printDefinitions(const std::vector<Definition>& definitions) {
    for (const auto& def : definitions) {
        std::cout << "Function: " << def.name << "\n";
        std::cout << "Full Line: " << def.line << "\n";
        std::cout << "Arguments:\n";
        for (const auto& arg : def.args) {
            std::cout << "  - Name: " << (arg.name.empty() ? "<none>" : arg.name)
                      << ", Type: " << arg.type << "\n";
        }

        std::cout << "--------------------------\n";
    }
}

int main() {
    // Replace with actual file paths if needed
    auto inputFilePath = std::filesystem::current_path() / "vortex/linux/syscall/sysdefs.c";

    // Read the file content into a string
    std::ifstream inputFile(inputFilePath);
    if (!inputFile) {
        std::cerr << "Error reading the file: " << inputFilePath << '\n';
        return 1;
    }

    std::string data((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Parse definitions from the file content
    auto definitions = parseDefinitions(data);

    // Print the parsed definitions and arguments
    // printDefinitions(definitions);

    auto wrapper     = createSyscallWrapper(definitions);
    if (std::ofstream outFile{"vortex/linux/syscall/wrapper.hpp"}; outFile) {
        outFile << wrapper; // Write the string to the file
    } else {
        std::cerr << "Error opening file for writing: wrapper.hpp" << std::endl;
    }

    std::system("clang-format -i vortex/linux/syscall/wrapper.hpp");

    return 0;
}
