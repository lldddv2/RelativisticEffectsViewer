const light = document.getElementById("ligth");
const star = document.getElementById("star");

// Cambiar la posición del vector
function changePosition(theta) {
  setArc(theta);
  theta = (theta * Math.PI) / 180; // Genera un ángulo aleatorio si no se proporciona
  const x = Math.sin(theta) * 50 + 50;
  const y = -Math.cos(theta) * 50 + 50;
  light.style.transform = `rotate(${theta}rad)`;
  light.style.left = `calc(${x}% - 9px`;
  light.style.top = `calc(${y}% - 28px)`;

  // Cambiar la posición de la estrella

  const xStar = Math.sin(theta) * 65 + 55;
  const yStar = -Math.cos(theta) * 65 + 60;

  star.style.left = `calc(${xStar}% - 9px)`;
  star.style.top = `calc(${yStar}% - 28px)`;
}

changePosition(0);

slider = document.getElementById("slider-interact");
vellabel = document.getElementById("vel-label");
anglelabel = document.getElementById("angle-label");
arrowImage = document.getElementById("arrow-image");

// on slider change, change the position of the light
slider.oninput = function () {
  const value = this.value;
  gamma = 1 / Math.sqrt(1 - value * value);
  tanTheta = gamma * value
  const theta = Math.atan(tanTheta) * (180 / Math.PI);
  anglelabel.innerHTML = "Angle = " + Math.round(theta) + "°";  
  changePosition(theta);
  vellabel.innerHTML = "Velocity = " + value + " c";

  arrowImage.src = "images/arrows/" + Math.round(value * 10) + ".png";
};

function showSection(id) {
  // Oculta solo las secciones del primer nivel
  document
    .querySelectorAll(".main-section")
    .forEach((el) => el.classList.add("d-none"));
  document.getElementById(id).classList.remove("d-none");
}

function showSubSection(id) {
  // Oculta solo las sub-secciones del segundo nivel
  document
    .querySelectorAll(".sub-section")
    .forEach((el) => el.classList.add("d-none"));
  document.getElementById(id).classList.remove("d-none");
}

function setArc(angle) {
  const arc = document.getElementById("angle");

  x = Math.cos(angle * (Math.PI / 180)) * 75 + 50;
  y = Math.sin(angle * (Math.PI / 180)) * 75 + 50;

  vertices = [
    [100, 50],
    [100, 100],
    [50, 100],
    [0, 100],
    [0, 50][(0, 0)],
    [50, 0],
    [100, 0],
    [100, 50],
  ];

  clipPath = `polygon(50% 50%,`;

  const segmentos = Math.floor(angle / 45) + 1;
  for (let i = 0; i < segmentos; i++) {
    const [x, y] = vertices[i];
    clipPath += `${x}% ${y}%,`;
  }

  clipPath += `${x}% ${y}%)`;
  console.log(clipPath);

  arc.style.clipPath = clipPath;
}

// <!-- insert light_transformation.json -->

function loadInteactive(graphDivId, jsonFilePath){
  fetch(jsonFilePath)
    .then((response) => {
      if (!response.ok) {
        throw new Error(
          `Error al cargar el archivo JSON: ${response.statusText}`
        );
      }
      return response.json(); // Parsea la respuesta como JSON
    })
    .then((figureAsJson) => {
      // 'figureAsJson' es el objeto figura completo (data, layout, frames)
      // Plotly.newPlot puede tomar directamente este objeto
      Plotly.newPlot(graphDivId, figureAsJson, {responsive: true});
    })
    .catch((error) => {
      console.error("Hubo un problema con la operación fetch:", error);
      document.getElementById(
        graphDivId
      ).innerHTML = `<p style="color:red; text-align:center;">Error al cargar el gráfico: ${error.message}</p>`;
    });
}

loadInteactive("plotlyDiv", "light_transformation.json");
loadInteactive("plotlyDiv2", "aberracion_relativista.json");
loadInteactive("plotlyDiv3", "photon_animation.json");
loadInteactive("plotlyDiv4", "synchronization_animation.json");