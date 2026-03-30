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
    python3Packages.mypy
    python3Packages.pre-commit-hooks
  ];
}
