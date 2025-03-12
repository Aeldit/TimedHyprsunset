# TimedHyprsunset

Toggles hyprland's hyprsunset at the start and stop times given as argument

## Usage

To use this program, make it start at the launch of your session / computer

On Hyprland for example, you would add the following line to you `hyprland.conf` file:

> `HH` is for the hours (ex: 18) and `MM` is for the minutes (ex: 57)
>
> The start time **MUST** be after the stop time. If it is not, the program will not behave as expected

```conf
exec-once = path/to/timed-sunset --start HH:MM --stop HH:MM
```

## Build

To build the project, use the following commands:

```sh
git clone https://github.com/Aeldit/TimedHyprsunset.git
cd TimedHyprsunset
make build
```

The file will then be located at `./timed-sunset`

