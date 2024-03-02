# Attach XTAG Device to WSL and run UDEV rules

# Function to convert Windows Path to WSL Path
function Convert-WindowsPathToWSLPath {
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

# Add UDEV rules
$wslPath = Convert-WindowsPathToWSLPath -WindowsPath $PWD.Path
$udevPath = "/etc/udev/rules.d"

$udevContents = wsl ls $udevPath
if ($udevContents -match "99-xmos.rules") {
    
} else {
    wsl sudo cp "$wslPath/scripts/99-xmos.rules" $udevPath 
    wsl sudo service udev reload
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

# Run the `usbipd wsl list` command and capture its output
$usbipdOutput = usbipd wsl list
# Use the Where-Object cmdlet to filter the list based on the DEVICE name
$filteredDevices = $usbipdOutput | Where-Object { $_ -match "XMOS XTAG-4" }
# Extract BUSID from the filtered list
$busids = $filteredDevices | ForEach-Object {
    # Split the line by whitespace and take the first element (BUSID)
    $busid = ($_ -split '\s+')[0]
    $busid
}
# Attach the busids to WSL
foreach ($busid in $busids) {
    usbipd wsl attach --busid $busid
}


