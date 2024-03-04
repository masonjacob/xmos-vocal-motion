#Requires -RunAsAdministrator

# Attach/Detach XTAG Device to WSL, run UDEV rules
function Convert-WindowsPathToWSLPath {
    # Function to convert Windows Path to WSL Path
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$WindowsPath
    )
    # Convert drive part to /mnt/<lowercase drive letter>/
    $lowercaseChar = $WindowsPath[0].toString()
    $lowercaseChar = $lowercaseChar.ToLower()
    $WSLPath = $lowercaseChar + $WindowsPath.Substring(1)
    $WSLPath = $WSLPath -replace '^([A-Z]):\\', '/mnt/$1/'
    # Convert backslashes to forward slashes
    $WSLPath = $WSLPath -replace '\\', '/'
    # Add \ before any spaces
    $WSLPath = $WSLPath -replace ' ', '\ '
    return $WSLPath
}

function Add-UDEV-Rules-To-WSL {

    # Add UDEV rules
    $wslPath = Convert-WindowsPathToWSLPath -WindowsPath $PWD.Path
    $udevPath = "/etc/udev/rules.d"

    $udevContents = wsl ls $udevPath
    if ($udevContents -match "99-xmos.rules") {
        
    } else {
        wsl sudo cp "$wslPath/scripts/99-xmos.rules" $udevPath 
        wsl sudo service udev reload
        # wsl udevadm control --reload
        # wsl udevadm trigger
    }

    # $tempPath = "/tmp/scripts"
    # wsl mkdir $tempPath
    # wsl cp "$wslPath\setup_xmos_devices.sh" $tempPath
    # wsl cp "$wslPath\check_xmos_devices.sh" $tempPath
    # wsl chmod +x $tempPath/setup_xmos_devices.sh
    # wsl chmod +x $tempPath/check_xmos_devices.sh
    # wsl sudo -e $tempPath/setup_xmos_devices.sh
    # wsl sudo -e $tempPath/check_xmos_devices.sh

    # wsl sudo rm $tempPath -r -f
}

function Find-XTAG-Devices {
    # Run the `usbipd wsl list` command and capture its output
    $usbipdOutput = usbipd list
    # Use the Where-Object cmdlet to filter the list based on the DEVICE name
    $filteredDevices = $usbipdOutput | Where-Object { $_ -match "XMOS XTAG-4" }
    # Extract BUSID from the filtered list
    $HardwareIDs = $filteredDevices | Select-String -Pattern '\b[a-fA-F0-9]{4}:[a-fA-F0-9]{4}\b' -AllMatches | ForEach-Object { $_.Matches.Value }
    Write-Host "Found XTAG device with Hardware-ID: $HardwareIDs"
    return $HardwareIDs
}
function Bind {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$HardwareID
    )
    usbipd bind --hardware-id $HardwareID
}

function Attach {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$HardwareID
    )
    usbipd attach --wsl --hardware-id $HardwareID
}

function AutoAttach {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$HardwareID
    )
    usbipd attach --wsl --auto-attach --hardware-id $HardwareID 
}

function Detach {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$HardwareID
    )
    usbipd detach --hardware-id $HardwareID
}

function Show-Usage {
    Write-Host 
"Usage: configure.ps1 [options?]`r`n
Options:`r
`t<none>`t`t add UDEV rules and attach XTAG device to WSL`r
`t--add-rules`t Add UDEV rules to WSL`r
`t--bind`t Bind XTAG device to WSL`r
`t--attach`t Attach XTAG device to WSL`r
`t--auto-attach`t Auto-Attach XTAG device to WSL`r
`t--detach`t Detach XTAG device from WSL`r
`t--help`t`t Show this help message`r`n"
    exit 1
}

try {
    # Check if the package is installed
    Get-Package -Name usbipd-win -ErrorAction Stop
} 
catch {
    # If Get-Package throws an error, the package is not installed
    Write-Host "Package usbipd is not installed. Installing..."

    # Install the package using winget
    winget install usbipd
}

$HardwareIDs = Find-XTAG-Devices
# Check if required parameters are provided

if ($args.Count -ne 0) {
    foreach ($HardwareID in $HardwareIDs) {
        foreach ($arg in $args) {
            if ($arg -eq "--bind") {
                Bind -HardwareID $HardwareID
            } elseif ($arg -eq "--attach") {
                Attach -HardwareID $HardwareID
            } elseif ($arg -eq "--auto-attach") {
                AutoAttach -HardwareID $HardwareID
            } elseif ($arg -eq "--detach") {
                Detach -HardwareID $HardwareID
            } elseif ($arg -eq "--add-rules") {
                Add-UDEV-Rules-To-WSL
            } else {
                Show-Usage
            }
        }
    }
}