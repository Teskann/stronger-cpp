import urllib.request
from pathlib import Path

current_dir = Path(__file__).parent
named_type_dir = current_dir / "NamedType"


def get_NamedType():
    named_type_dir.mkdir(exist_ok=True)

    to_download = ["crtp.hpp", "named_type.hpp", "named_type_impl.hpp", "underlying_functionalities.hpp", "version.hpp"]

    for file in to_download:
        target_file = named_type_dir / file
        if not target_file.exists():
            print(f"Downloading {file} to {target_file}")
            urllib.request.urlretrieve(
                f"https://raw.githubusercontent.com/joboccara/NamedType/master/include/NamedType/{file}", target_file)


if __name__ == "__main__":
    get_NamedType()