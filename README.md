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
| Build target | Android64 (arm64-v8a) |

The Geode SDK version is specified in `mod.json`.

## Build with GitHub Actions

The repository includes a workflow at `.github/workflows/build.yml`.

The workflow runs automatically when a commit is pushed to the `main` branch. It can also be started manually:

1. Open the repository's **Actions** tab.
2. Select **Build Dynamic Icon**.
3. Press **Run workflow** and choose the `main` branch.
4. Wait for the **Build Android64** job to finish.
5. Open the workflow run and download the `Dynamic-Icon-Android64` artifact.
6. Extract the downloaded ZIP archive to get the `.geode` package.

The workflow runs on GitHub-hosted Ubuntu and uses `geode-sdk/build-geode-mod@main` to build the mod with the Geode SDK. The build target is `Android64` and the build configuration is `Release`. The resulting package is uploaded as the `Dynamic-Icon-Android64` artifact. If the build does not produce a package, the artifact upload step fails.

## Local build

Install the Geode CLI, Android NDK and the matching Geode SDK. Then run:

```sh
geode sdk install v6.0.0-beta.2
geode sdk install-binaries -p android64 --version v6.0.0-beta.2
geode build -p android64
```

## License

MIT. See [LICENSE](LICENSE).
