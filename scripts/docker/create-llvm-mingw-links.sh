#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Error: No target architecture provided."
    exit 1
fi

create_ld() {
  filename=bin/$TARGET-ld
  rm $filename
  echo "#!/bin/bash" > $filename
  echo "ld.lld \"\$@\"" >> $filename
  chmod +x $filename
}

TARGET="$1"
SEARCH_DIR="."

find "$SEARCH_DIR" -type l -name 'aarch64-w64-mingw32-*' | while read -r symlink; do
  base_name=$(basename "$symlink")
  new_base_name="$TARGET-${base_name#*aarch64-w64-mingw32-}"
  dir_path=$(dirname "$symlink")
  target=$(readlink "$symlink")
  ln -s "$target" "${dir_path}/${new_base_name}"
done

create_ld