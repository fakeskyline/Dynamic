# Dynamic Icon

Dynamic Icon adds a shortcut to the official Geometry Dash garage on the in-level pause screen. It also unlocks icon and color selections and applies the selected appearance when returning from the garage.

## Features

- Opens the official garage from the pause menu without resuming the level.
- Keeps the gameplay update loop frozen while the garage is open.
- Returns to the existing pause screen when the garage is closed.
- Applies the selected icon for the player's current game mode.
- Applies the selected primary color, secondary color and Glow.
- Reports icons and colors as unlocked.
- Reports the Practice Music Sync item as unlocked.

The mod uses the game's official garage interface. It does not add a separate icon grid or a custom icon-selection screen.

## Hooks

| Class | Hooked methods | Purpose |
| --- | --- | --- |
| `PauseLayer` | `customSetup`, `onResume` | Adds the garage button and clears the freeze state when gameplay resumes. |
| `GJGarageLayer` | `onBack`, `keyBackClicked` | Returns to the paused level after closing the garage. |
| `GJBaseGameLayer` | `update` | Stops gameplay updates while the garage is open from a paused level. |
| `GameManager` | `isIconUnlocked`, `isColorUnlocked` | Makes icon and color unlock checks return true. |
| `GameStatsManager` | `isItemUnlocked` | Unlocks Practice Music Sync (GJItem 17); other items use the original method. |
| `PlayLayer` | `resetLevel`, `onQuit` | Clears the freeze state when the level is reset or exited. |

When returning from the garage, the mod reads the active mode from the live `PlayerObject` and applies its icon frame, colors and Glow. The mode is not taken from the garage tab.

## Compatibility

| Platform | Architecture | Geode target | Binary |
| --- | --- | --- | --- |
| Android32 | ARMv7 | `armeabi-v7a` / `Android32` | `fakeskyline.dynamic.android32.so` |
| Android64 | ARM64 | `arm64-v8a` / `Android64` | `fakeskyline.dynamic.android64.so` |
| Windows | x86_64 | `Win64` | `fakeskyline.dynamic.dll` |

Each platform is compiled independently with its own Geode target and toolchain. The gameplay source is shared because it uses cross-platform Geode APIs; platform-specific source is only needed if platform-specific functionality is introduced.

- Geometry Dash: 2.2091
- Geode SDK: v6.0.0-beta.2

## GitHub Actions builds

The workflow at `.github/workflows/build.yml` builds Android32, Android64 and Win64 in separate matrix jobs. Each build uses `geode-sdk/build-geode-mod@main` with `combine: true`.

After all three builds succeed, the official `geode-sdk/build-geode-mod/combine@main` action merges the platform binaries into one universal `.geode` package.

The workflow then validates the package before uploading it:

- Checks that all three platform binaries are present.
- Checks Android32 is ELF32 / ARM.
- Checks Android64 is ELF64 / AArch64.
- Checks Windows is PE32+ / x86-64.

If a binary is missing or has the wrong architecture, validation fails and the universal artifact is not published. The artifact is named `Dynamic-Icon-All-Platforms` and is retained for 30 days.

Pushes to `main` start the workflow automatically. It can also be started manually from the repository's **Actions** tab.

## Local build

Install the Geode CLI and matching SDK binaries, then build the target you need:

```sh
geode sdk install v6.0.0-beta.2
geode sdk install-binaries -p android32 --version v6.0.0-beta.2
geode build -p android32

geode sdk install-binaries -p android64 --version v6.0.0-beta.2
geode build -p android64

geode sdk install-binaries -p win --version v6.0.0-beta.2
geode build -p win
```

## License

MIT. See [LICENSE](LICENSE).
