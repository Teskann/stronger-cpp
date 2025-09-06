import pathlib
import re

here = pathlib.Path(__file__).parent.absolute()
headers_dir = here / "includes"


def get_all_headers():
    res = {}
    for file in headers_dir.rglob("*.hpp"):
        res[str(file.relative_to(headers_dir).as_posix())] = file.read_text().split("#pragma once\n")[1] + "\n\n"
    return res


def get_includes(header_content):
    include_regex = re.compile('(#include "(.*?)")')
    all_includes = include_regex.findall(header_content)
    return all_includes


def replace_includes(header_content, all_headers):
    includes = get_includes(header_content)
    already_replaced = set()

    while includes:
        for include in includes:
            replacement = all_headers[include[1]] if include[1] not in already_replaced else ""
            header_content = header_content.replace(include[0], replacement)
            already_replaced.add(include[1])
        includes = get_includes(header_content)
    return header_content


def get_license():
    license_file = pathlib.Path(__file__).parent / "LICENSE"
    lic = license_file.read_text()
    lic = "// " + lic.replace("\n", "\n// ")
    return lic + "\n\n"


def add_license(header_content):
    return f"#pragma once\n".join([get_license()] + header_content.split("#pragma once\n")[1:])


def put_includes_at_top(header_content):
    include_regex = re.compile('(#include .*)')
    all_includes = include_regex.findall(header_content)
    header_content = re.sub(include_regex, "", header_content)
    header_parts = header_content.split("#pragma once\n")
    header_parts.insert(1, f"#pragma once\n\n{'\n'.join(set(all_includes))}\n\n")
    return "".join(header_parts)


def limit_newlines(header_content):
    return re.sub(r'\n{2,}', '\n\n', header_content)


if __name__ == "__main__":
    print("Making header-only version of stronger-cpp ...")
    main_header = headers_dir / "stronger.hpp"
    all_headers = get_all_headers()
    header_only_content = replace_includes(main_header.read_text(), all_headers)
    header_only_content = put_includes_at_top(header_only_content)
    header_only_content = limit_newlines(header_only_content)
    header_only_content = add_license(header_only_content)
    public_dir = here / "public"
    public_dir.mkdir(exist_ok=True)
    target_file = (public_dir / main_header.name)
    target_file.write_text(header_only_content)
    print(f"Header-only version of stronger-cpp created at {target_file}")
