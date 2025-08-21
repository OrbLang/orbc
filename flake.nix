{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    zig.url = "github:mitchellh/zig-overlay";
  };

  outputs =
    {
      nixpkgs,
      zig,
      ...
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        overlays = [
          zig.overlays.default
        ];
      };
    in
    {
      devShell.${system} = import ./nix/devShell.nix { inherit pkgs; };
      formatter.${system} = pkgs.nixfmt-rfc-style;
    };
}
