#include <iostream>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <map>

namespace fs = std::filesystem;

std::string generate_random_string(size_t length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }
    return random_string;
}

void create_directory_with_files(const std::string& dir_name, int file_count) {
    try {
        // Create directory
        fs::create_directory(dir_name);

        // Create files with random names and content
        for (int i = 0; i < file_count; ++i) {
            std::string file_name = generate_random_string(8) + ".txt";
            std::ofstream file(dir_name + "/" + file_name);
            file << generate_random_string(100); // Random content
            file.close();
        }

        // Iterate over files and print their names and sizes
        for (const auto& entry : fs::directory_iterator(dir_name)) {
            if (entry.is_regular_file()) {
                std::cout << "File: " << entry.path().filename() << ", Size: " << fs::file_size(entry.path()) << " bytes\n";
            }
        }

        // Create subdirectory and copy files
        std::string sub_dir_name = dir_name + "/backup";
        fs::create_directory(sub_dir_name);
        for (const auto& entry : fs::directory_iterator(dir_name)) {
            if (entry.is_regular_file()) {
                fs::copy(entry.path(), sub_dir_name + "/" + entry.path().filename().string());
                fs::remove(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void analyze_directory(const std::string& dir_name) {
    try {
        size_t total_files = 0;
        uintmax_t total_size = 0;
        std::map<std::string, size_t> file_types;

        for (const auto& entry : fs::directory_iterator(dir_name)) {
            if (entry.is_regular_file()) {
                ++total_files;
                total_size += fs::file_size(entry.path());

                std::string extension = entry.path().extension().string();
                ++file_types[extension];
            }
        }

        std::cout << "Total files: " << total_files << '\n';
        std::cout << "Total size: " << total_size << " bytes\n";
        std::cout << "File types distribution:\n";
        for (const auto& [extension, count] : file_types) {
            std::cout << extension << ": " << count << " files\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

int main() {
    std::string dir_name;
    int file_count;

    std::cout << "Enter directory name: ";
    std::cin >> dir_name;
    std::cout << "Enter number of files to create: ";
    std::cin >> file_count;

    create_directory_with_files(dir_name, file_count);

    std::cout << "\nAnalyzing directory...\n";
    analyze_directory(dir_name + "/backup");

    return 0;
}
