let colorImage = document.getElementById("catImage");
let button2 = document.getElementById("buttonCat");

function changeToOtherCat() {

	if (colorImage.getAttribute('src') === "chat_potte.jpg") {
		colorImage.setAttribute('src', "chat_mignon.webp");
	}
	else {
		colorImage.setAttribute('src', "chat_potte.jpg");
	}
}

button2.addEventListener("click", changeToOtherCat);
