#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
import pathlib
from dataclasses import dataclass

fakeit_path = os.path.join(os.path.dirname(__file__), "..")


@dataclass
class ProjectStructure:
    gen_dir: os.PathLike
    single_header_dir: os.PathLike
    files: int
    interfaces: int
    methods: int
    stubs: int


def create_project(project_structure: ProjectStructure):
    pathlib.Path(project_structure.gen_dir).mkdir(parents=True, exist_ok=True)
    # Test file
    for file_idx in range(0, project_structure.files):
        with open(
            os.path.join(project_structure.gen_dir, f"file_{file_idx}.cpp"), "w"
        ) as file:
            file.write("#include <fakeit.hpp>\n")
            file.write(f'#include "file_{file_idx}.hpp"\n')
            file.write("using namespace fakeit;\n")
            file.write("namespace {\n")
            # Interface
            for interface_idx in range(0, project_structure.interfaces):
                file.write(f"class Interface_{interface_idx} {{\n")
                file.write("public:\n")
                # Method
                for method_idx in range(0, project_structure.methods):
                    file.write(f"    virtual int method_{method_idx}(int) = 0;\n")
                file.write("};\n")
                # Test of methods
                file.write(f"void test_interface_{interface_idx}() {{\n")
                file.write(f"    Mock<Interface_{interface_idx}> mock;\n")
                for method_idx in range(0, project_structure.methods):
                    for stub_idx in range(0, project_structure.stubs):
                        file.write(
                            f"    When(Method(mock, method_{method_idx}).Using({stub_idx})).Return({stub_idx});\n"
                        )
                for method_idx in range(0, project_structure.methods):
                    for stub_idx in range(0, project_structure.stubs):
                        file.write(f"    mock.get().method_{method_idx}({stub_idx});\n")
                for method_idx in range(0, project_structure.methods):
                    for stub_idx in range(0, project_structure.stubs):
                        file.write(
                            f"    Verify(Method(mock, method_{method_idx}).Using({stub_idx})).Exactly(1);\n"
                        )
                file.write("}\n")
            file.write("}\n")
            # Test of whole file
            file.write(f"void test_file_{file_idx}() {{\n")
            for interface_idx in range(0, project_structure.interfaces):
                file.write(f"    test_interface_{interface_idx}();\n")
            file.write("}\n")
        # Header of test file
        with open(
            os.path.join(project_structure.gen_dir, f"file_{file_idx}.hpp"), "w"
        ) as file:
            file.write("#pragma once\n")
            file.write(f"void test_file_{file_idx}();\n")
    # Main
    with open(os.path.join(project_structure.gen_dir, "main.cpp"), "w") as file:
        for file_idx in range(0, project_structure.files):
            file.write(f'#include "file_{file_idx}.hpp"\n')
        file.write("int main() {\n")
        for file_idx in range(0, project_structure.files):
            file.write(f"    test_file_{file_idx}();\n")
        file.write("}\n")
    # CMake file
    with open(os.path.join(project_structure.gen_dir, "CMakeLists.txt"), "w") as file:
        file.write("cmake_minimum_required(VERSION 3.14)\n")
        file.write("project(FakeIt-bench LANGUAGES CXX)\n")
        file.write("add_executable(bench\n")
        for file_idx in range(0, project_structure.files):
            file.write(f"    file_{file_idx}.cpp\n")
        file.write(f"    main.cpp\n")
        file.write(")\n")
        file.write(
            f'target_include_directories(bench PRIVATE "{project_structure.single_header_dir}")\n'
        )


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "directory",
        type=str,
        help="Directory where the benchmark project will be created.",
    )
    parser.add_argument(
        "-d",
        "--header-dir",
        type=str,
        default=os.path.join(fakeit_path, "single_header", "standalone"),
        help="The directory containing the fakeit single header.",
    )
    parser.add_argument(
        "-f", "--files", type=int, default=10, help="Number of C++ files to create."
    )
    parser.add_argument(
        "-i",
        "--interfaces",
        type=int,
        default=10,
        help="Number of interfaces per file.",
    )
    parser.add_argument(
        "-m", "--methods", type=int, default=10, help="Number of methods per interface."
    )
    parser.add_argument(
        "-s", "--stubs", type=int, default=10, help="Number of stubs per methods."
    )
    return parser


def main():
    args = get_parser().parse_args()
    create_project(
        ProjectStructure(
            args.directory,
            args.header_dir,
            args.files,
            args.interfaces,
            args.methods,
            args.stubs,
        )
    )


if __name__ == "__main__":
    main()
