<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
	</head>
	<body>
		<?php
		if ($_SERVER['REQUEST_METHOD'] === 'POST')
		{
			$name = htmlspecialchars($_POST['nom']);
			$age = (int)$_POST['age'];
			echo "Bonjour, $name .\r\n";
			echo "Tu as $age  ans.";
		}
		else
		{
			$name = htmlspecialchars($_GET['nom']);
			$age = (int)$_GET['age'];
			echo "Bonjour, $name .\r\n";
			echo "Tu as $age  ans.";
		}
		?>
	</body>
</html>