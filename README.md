# Dynamic Icon

Dynamic Icon adds an icon picker to the Geometry Dash in-level pause menu. The editor is designed to let players change their selected game mode icon without leaving the current level.

## Current implementation

- Pause-menu entry point.
- Icon categories: Cube, Ship, Ball, UFO, Wave, Robot, Spider, Swing and Jetpack.
- Paged icon grid with in-game icon previews.
- Selecting an icon updates the corresponding player icon through GameManager.

## Compatibility

| Component | Version |
| --- | --- |
| Geometry Dash | 2.2091 |
| Geode SDK | v6.0.0-beta.2 |
| Build target | Android64 (arm64-v8a) |

The project metadata pins the exact Geode SDK version. The Android build uses the official Geode build action.

## Build with GitHub Actions

1. Open the repository's **Actions** tab.
2. Select **Build Dynamic Icon**.
3. Run the workflow, or push a commit to `main`.
4. Open the completed workflow run and download the `Dynamic-Icon-Android64` artifact.
5. Extract the ZIP archive to obtain the `.geode` package.

## Local build

Install the Geode CLI and Android NDK, then install the matching SDK and Android64 binaries:

```sh
geode sdk install v6.0.0-beta.2
geode sdk install-binaries -p android64 --version v6.0.0-beta.2
geode build -p android64
```

The generated package is placed in the Android64 build directory.

## License

MIT. See [LICENSE](LICENSE).
