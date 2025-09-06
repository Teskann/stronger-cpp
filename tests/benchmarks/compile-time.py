import time
from pathlib import Path
import subprocess

CURRENT_DIR = Path(__file__).parent


def compile_benchmark(iterations, *defines):
    compile_options = ["compile-time.cpp", "-o", "compile-time", "-std=c++23", "-O3"]
    command = ["g++"] + compile_options + [f"-D{define}" for define in defines]
    print("Running:", *command, sep=" ")

    start = time.time()
    for i in range(iterations):
        result = subprocess.run(command, cwd=CURRENT_DIR)
        if result.returncode != 0:
            raise RuntimeError("Compilation failed")
    stop = time.time()
    delta = stop - start
    print(f"Time taken: {delta} seconds\n")
    return delta


def compile_all(name, iterations, *define):
    print(15 * "=", f"{name.upper()}", 15 * "=")
    print(f"Compiling {name} (include only)...")
    header = compile_benchmark(iterations, "INCLUDE_ONLY", *define)
    print(f"Compiling {name} ...")
    full = compile_benchmark(iterations, *define)
    print("=" * 30, "\n", sep="")
    return header, full


def compile_named_type(iterations):
    return compile_all("NamedType", iterations, "COMPILE_TIME_BENCHMARK_NAMED_TYPE")


def compile_stronger(iterations):
    return compile_all("Stronger", iterations, "COMPILE_TIME_BENCHMARK_STRONGER")


def compile_no_strong_type(iterations):
    return compile_all("NoStrongType", iterations, "INCLUDE_ONLY")


def render_results(settings):
    template = CURRENT_DIR / "results.md.template"
    content = template.read_text(encoding="utf-8")
    for place_holder, value in settings.items():
        content = content.replace(place_holder, str(value))
    output_file = CURRENT_DIR / "results.md"
    output_file.write_text(content, encoding="utf-8")


def get_y_max(*values):
    from math import ceil
    return ceil(max(values) * 1.1)


def get_performance(first, second):
    if first > second:
        return get_performance(second, first).replace("faster", "slower")
    faster = second / first
    if faster >= 2:
        return f"{round(faster, 1)} times faster"
    faster = (second - first) / first
    return f"{round(faster * 100)}% faster"


if __name__ == "__main__":
    ITERATIONS = 10
    settings = {}

    named_type_header, named_type_full = compile_named_type(ITERATIONS)
    stronger_header, stronger_full = compile_stronger(ITERATIONS)
    no_strong_type_header, no_strong_type_full = compile_no_strong_type(ITERATIONS)

    settings["__N_ITERATIONS__"] = ITERATIONS
    settings["__MAX_Y_HEADER__"] = get_y_max(named_type_header, stronger_header, no_strong_type_header)
    settings["__MAX_Y_FULL__"] = get_y_max(named_type_full, stronger_full, no_strong_type_full)
    settings["__NO_STRONG_TYPE_HEADER__"] = no_strong_type_header
    settings["__NO_STRONG_TYPE_FULL__"] = no_strong_type_full
    settings["__NAMED_TYPE_HEADER__"] = named_type_header
    settings["__NAMED_TYPE_FULL__"] = named_type_full
    settings["__STRONGER_CPP_HEADER__"] = stronger_header
    settings["__STRONGER_CPP_FULL__"] = stronger_full
    settings["__STRONGER_CPP_VS_NAMED_TYPE_HEADER__"] = get_performance(stronger_header, named_type_header)
    settings["__STRONGER_CPP_VS_NAMED_TYPE_FULL__"] = get_performance(stronger_full, named_type_full)
    settings["__STRONGER_CPP_VS_NO_STRONG_TYPE_HEADER__"] = get_performance(stronger_header, no_strong_type_header)
    settings["__STRONGER_CPP_VS_NO_STRONG_TYPE_FULL__"] = get_performance(stronger_full, no_strong_type_full)

    render_results(settings)
