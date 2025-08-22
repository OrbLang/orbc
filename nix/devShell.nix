{ pkgs, ... }:
pkgs.mkShellNoCC {
  packages = with pkgs; [
    #(pkgs.callPackage ./zig_0_15_1.nix {})
    zig_0_14
  ];

  shellHook = ''
    echo -e "---------DEVSHELL---------\\n\\n"
  '';
}
