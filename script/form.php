<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
	</head>
	<body>
		Bonjour, <?php echo htmlspecialchars($_POST['nom']); ?>.<br>
		Tu as <?php echo (int)$_POST['age']; ?> ans.
	</body>
</html>