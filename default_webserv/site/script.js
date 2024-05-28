document.addEventListener("DOMContentLoaded", function() {
	function changeCat() {
		var img = document.getElementById('catImage');
		if (img.src.includes("chat_potte")) {
			img.src = "chat_mignon.webp";
		} else {
			img.src = "chat_potte.jpg";
		}
	}
	document.querySelector('button').addEventListener('click', changeCat);
});
