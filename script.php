<?php
$time_start = microtime(true);

$jsonData = [
    "name" => "Example Project",
    "version" => "1.0.0",
    "isActive" => true,
    "settings" => [
        "theme" => "dark",
        "notifications" => true,
        "language" => "en"
    ]
];

// $jsonFile = 'input.json';
$outputFile =  Date('Y-m-d') . '.json';
$shouldEncrypt = true;
$encryptionKey = "12345678901234567890123456789012";
$shouldMinimize = true;

// Convert PHP array to JSON string and escape special characters
$jsonString = json_encode($jsonData, JSON_UNESCAPED_SLASHES);

// Build the command with proper escaping and spacing
$command = sprintf(
    'json-parser %s %s%s%s',
    // escapeshellarg($jsonFile),
    escapeshellarg($jsonString),
    escapeshellarg($outputFile),
    $shouldMinimize ? ' --minimize' : '',
    $shouldEncrypt ? ' --encrypt ' . escapeshellarg($encryptionKey) : ''
);

// Execute command and capture output
exec($command . ' 2>&1', $output, $return_var);
echo ($command);

// Check if the execution was successful
if ($return_var == 0) {
    echo "C++ Driver executed successfully.\n";
} else {
    echo "Failed to execute C++ Driver.\n";
    echo "Error: " . implode("\n", $output) . "\n";
    echo "Command used: " . $command . "\n"; // Added for debugging
}

// Add new function to check file status
function checkFileStatus($filePath)
{
    $command = sprintf('json-parser --check %s', escapeshellarg($filePath));
    exec($command, $output, $return_var);

    if ($return_var == 0 && !empty($output)) {
        return $output[0];
    }
    return "Error checking file status";
}

// Example usage:
$fileToCheck = $outputFile; // or any other file you want to check
$status = checkFileStatus($fileToCheck);
echo "File status: " . $status . "\n";

$time_end = microtime(true);
$time = $time_end - $time_start;
echo "Time taken: " . $time . " seconds\n";
