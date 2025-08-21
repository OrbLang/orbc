{ pkgs, ... }:
pkgs.mkShellNoCC {
  packages = [
    #(pkgs.callPackage ./zig_0_15_1.nix {})
    pkgs.zig_0_14
    pkgs.zsh
  ];

  shellHook = ''
    echo -e "---------DEVSHELL---------\\n\\n"
    exec zsh
  '';
}
