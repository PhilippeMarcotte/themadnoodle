Menu, Tray, Icon, ./madnoodle.png
sv := "./soundvolumeview-x64/SoundVolumeView.exe"
deviceSwitch := True
volumeSwitch := True
F13::
Run %sv% /ChangeVolume FocusedName 2
Return
F14::
Run %sv% /ChangeVolume FocusedName -2
Return
F15::
Run %sv% /ChangeVolume "Google Chrome" 2
Return
F16::
Run %sv% /ChangeVolume "Google Chrome" -2
Return
F17::
deviceSwitch := !deviceSwitch
If (deviceSwitch)
{
    Run %sv% /SetDefault Speakers all
}
Else
{
    Run %sv% /SetDefault Headphones all
}
Return
F18::
deviceSwitch := !deviceSwitch
If (deviceSwitch)
{
    Run %sv% /SetVolume AllAppVolume 20
}
Else
{
    Run %sv% /SetDefault AllAppVolume 0
}
Return
