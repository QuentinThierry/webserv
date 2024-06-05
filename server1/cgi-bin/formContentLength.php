<?php
$before = <<<EOF
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
	</head>
	<body>
EOF;

$after = <<<EOF
	</body>
</html>
EOF;

if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
	$name = htmlspecialchars($_POST['nom']);
	$age = (int)$_POST['age'];
	$data = $before . "Bonjour, $name .\r\nTu as $age  ans." . $after;
	$content_length = strlen($data);
	header("content-length: $content_length");
	echo $data;
}
else
{
	$name = htmlspecialchars($_GET['nom']);
	$age = (int)$_GET['age'];
	$data = $before . "Bonjour, $name .\r\nTu as $age  ans." . $after;
	$content_length = strlen($data);
	header("content-length: $content_length");
	echo $data;
}
?>