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

| Platform | Architecture | Geode target |
| --- | --- | --- |
| Android32 | armeabi-v7a | `Android32` |
| Android64 | arm64-v8a | `Android64` |
| Windows | x86_64 | `Win64` |

The project uses Geode APIs and Cocos2d interfaces shared by these targets. There are no Android-only or Windows-only calls in the gameplay code; the same source is compiled separately for each platform.

- Geometry Dash: 2.2091
- Geode SDK: v6.0.0-beta.2

## Build with GitHub Actions

The workflow is in `.github/workflows/build.yml`. It runs on pushes to `main` and can also be started manually from the repository's **Actions** tab.

Three platform builds run in parallel: Android32 and Android64 on Ubuntu, and Windows on a Windows runner. After all three succeed, a packaging job collects their `.geode` files into one artifact:

`Dynamic-Icon-All-Platforms`

To get all builds, open a completed workflow run, go to **Artifacts**, and download that single ZIP. It contains the packages for Android32, Android64 and Windows. The temporary per-platform artifacts are retained for one day; the combined artifact is retained for 30 days.

The workflow uses `geode-sdk/build-geode-mod@main`, the SDK version from `mod.json`, and the `Release` configuration. If any platform build fails, the combined package job will not run.

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
