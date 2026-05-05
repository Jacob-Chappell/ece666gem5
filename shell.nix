with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "gem5";
  buildInputs = [
    m4
    scons
    boost
    capstone
    gperftools
    hdf5-cpp
    libpng
    protobuf_21
    zlib
    pre-commit
    python3
    python3Packages.mypy
    python3Packages.pre-commit-hooks
    mold-wrapped
    python3Packages.pip
    # gcc13
    qemu
    packer
    binutils
    # glibc.static
  ];
}
