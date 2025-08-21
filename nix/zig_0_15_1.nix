{ stdenvNoCC, ... }:
stdenvNoCC.mkDerivation {
  pname = "zig_0_15_1";
  version = "0.15.1";
  src = fetchTarball {
    url = "https://ziglang.org/download/0.15.1/zig-x86_64-linux-0.15.1.tar.xz";
    sha256 = "0yar24a1qjg503czwnkdipky1cfb08k0mm9c8gwv827d33df1070";
  };

  phases = [ "installPhase" ];

  installPhase = ''
    mkdir -p $out/bin
    cp $src/zig $out/bin
  '';
}
