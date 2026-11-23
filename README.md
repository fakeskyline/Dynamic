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

The implementation uses Geode's `$modify` hooks on these game classes:

| Class | Hooked methods | Purpose |
| --- | --- | --- |
| `PauseLayer` | `customSetup`, `onResume` | Adds the garage button to the pause menu and clears the freeze state when gameplay resumes. |
| `GJGarageLayer` | `onBack`, `keyBackClicked` | Returns to the pause screen when the garage was opened from a paused level. |
| `GJBaseGameLayer` | `update` | Stops gameplay updates while the official garage is open from a paused level. |
| `GameManager` | `isIconUnlocked`, `isColorUnlocked` | Makes icon and color unlock checks return true. |
| `GameStatsManager` | `isItemUnlocked` | Makes the Practice Music Sync item (GJItem 17) return unlocked; other items use the original method. |
| `PlayLayer` | `resetLevel`, `onQuit` | Clears the freeze state when the level is reset or exited. |

After returning from the garage, the mod reads the active game mode from the live `PlayerObject`, then updates that player's icon frame, colors and Glow. This avoids using the garage's currently selected tab as the player's in-level game mode.

## Compatibility

| Component | Version |
| --- | --- |
| Geometry Dash | 2.2091 |
| Geode SDK | v6.0.0-beta.2 |
| Android | Android32 (armeabi-v7a), Android64 (arm64-v8a) |
| Windows | Win64 |

## Build with GitHub Actions

The workflow is located at `.github/workflows/build.yml`. It runs on pushes to `main` and can also be started manually from the repository's **Actions** tab.

Three builds run in parallel:

| Job | Runner | Geode target | Artifact |
| --- | --- | --- | --- |
| Android32 | Ubuntu | `Android32` | `Dynamic-Icon-Android32` |
| Android64 | Ubuntu | `Android64` | `Dynamic-Icon-Android64` |
| Windows | Windows | `Win64` | `Dynamic-Icon-Windows` |

To download a build, open the completed workflow run, find **Artifacts**, and download the package for your platform. Each artifact is a ZIP containing the corresponding `.geode` file.

The workflow uses `geode-sdk/build-geode-mod@main`, the Geode SDK version specified in `mod.json`, and the `Release` build configuration. Each platform is built independently, so a failure on one platform does not cancel the other builds.

## Local build

Install the Geode CLI and the matching SDK binaries, then run the command for your platform:

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
