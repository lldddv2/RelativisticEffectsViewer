var Module = {
  canvas: (function () {
    return document.getElementById("canvas");
  })(),
  print: function (text) {
    console.log(text); // Muestra en la consola del navegador

    // Muestra en la página (opcional)
    let outputDiv = document.getElementById("terminal-output");
    if (outputDiv) {
      outputDiv.innerHTML += "> " + text + "<br>";
    }
  },
};

document.addEventListener("keydown", function (event) {
  if (event.altKey && event.key === "e") {
    event.preventDefault(); // Bloquea la acción del navegador
    console.log("Alt + E presionado"); // Tu función aquí
  } else if (event.altKey && event.key === "c") {
    event.preventDefault(); // Bloquea la acción del navegador
    console.log("Alt + D presionado"); // Tu función aquí
  } else if (event.altKey && event.key === "s") {
    event.preventDefault(); // Bloquea la acción del navegador
    console.log("Alt + J presionado"); // Tu función aquí
  }
});

let up = document.getElementById("arrow-up");
let down = document.getElementById("arrow-down");
let left = document.getElementById("arrow-left");
let right = document.getElementById("arrow-right");
let space = document.getElementById("space");

let labelUp = document.getElementById("label-up");
let labelDown = document.getElementById("label-down");
let labelLeft = document.getElementById("label-left");
let labelRight = document.getElementById("label-right");
let labelSpace = document.getElementById("label-space");

let stopTime = document.getElementById("stop-time");

// sin control
let texts = {
  up: "RA",
  down: "RA",
  left: "DEC",
  right: "DEC",
  space: "Stop time",
};

let textsCtrl = {
  up: "Vel",
  down: "Vel",
  left: "Time",
  right: "Time",
  space: "Stop time",
};

// if ctrl is pressed put textsCtrl else texts

document.addEventListener("keydown", function (event) {
  if (event.ctrlKey) {
    labelUp.innerHTML = textsCtrl["up"];
    labelDown.innerHTML = textsCtrl["down"];
    labelLeft.innerHTML = textsCtrl["left"];
    labelRight.innerHTML = textsCtrl["right"];
  }
});

document.addEventListener("keyup", function (event) {
  if (!event.ctrlKey) {
    labelUp.innerHTML = texts["up"];
    labelDown.innerHTML = texts["down"];
    labelLeft.innerHTML = texts["left"];
    labelRight.innerHTML = texts["right"];
  }
});

// mientras presione una tecla, agreaga .active a la tecla
document.addEventListener("keydown", function (event) {
  if (event.key === "ArrowUp") {
    up.classList.add("active");
  } else if (event.key === "ArrowDown") {
    down.classList.add("active");
  } else if (event.key === "ArrowLeft") {
    left.classList.add("active");
  } else if (event.key === "ArrowRight") {
    right.classList.add("active");
  } else if (event.key === " ") {
    space.classList.add("active");
  }
});

// cuando space cambiar "Stop time" a "Resume time"
let spacePressed = false;

document.addEventListener("keydown", function (event) {
  if (event.key === " " && !spacePressed) {
    spacePressed = true;
    if (labelSpace.innerHTML === "Stop time") {
      labelSpace.innerHTML = "Resume time";
      stopTime.classList.remove("ph-pause-circle");
      stopTime.classList.add("ph-play-circle");
    } else {
      labelSpace.innerHTML = "Stop time";
      stopTime.classList.remove("ph-play-circle");
      stopTime.classList.add("ph-pause-circle");
    }
  }
});

document.addEventListener("keyup", function (event) {
  if (event.key === " ") {
    spacePressed = false;
  }
});

document.addEventListener("keyup", function (event) {
  if (event.key === "ArrowUp") {
    up.classList.remove("active");
  } else if (event.key === "ArrowDown") {
    down.classList.remove("active");
  } else if (event.key === "ArrowLeft") {
    left.classList.remove("active");
  } else if (event.key === "ArrowRight") {
    right.classList.remove("active");
  } else if (event.key === " ") {
    space.classList.remove("active");
  }
});

let terminalOutput = document.getElementById("terminal-output");
let terminalOutputButton = document.getElementById("terminal-output-button");
let terminalOutputVisible = true;
terminalOutputButton.addEventListener("click", function () {
  if (terminalOutputVisible) {
    terminalOutput.style.display = "none";
  } else {
    terminalOutput.style.display = "block";
  }
  terminalOutputVisible = !terminalOutputVisible;
});



class Vector3 {
  constructor(x, y, z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  dot(v) {
    return this.x * v.x + this.y * v.y + this.z * v.z;
  }

  norm() {
    return Math.sqrt(this.dot(this));
  }

  cross(v){
    return new Vector3(
      this.y * v.z - this.z * v.y,
      this.z * v.x - this.x * v.z,
      this.x * v.y - this.y * v.x
    );
  }

  angleToPlane(normal) {
    const cosPhi = Math.abs(this.dot(normal)) / (this.norm() * normal.norm());
    return Math.asin(cosPhi);
  }

  update(x, y, z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
}

const plane = document.getElementById("plane");
const height = document.getElementById("height");
const controls = document.getElementById("controls");


let numObjects = 0;
class objectManager {
  constructor(name, iconClassName, R) {
    this.name = name;
    this.upperName = name.charAt(0).toUpperCase() + name.slice(1);
    this.iconClassName = iconClassName;
    this.R = R;
    this.defectR = new Vector3(this.R.x, this.R.y, this.R.z);

    this.addToList();
    this.addToPlane();


    // set invisible
    this.plane.style.display = "none";
    this.height.style.display = "none";

    this.updatePlane();
  }

  addToList(){
    // <div class="form-check">
    this.parent = document.createElement("div");
    this.parent.className = "form-check";

    // <input class="form-check-input" type="checkbox" value="" id="flexCheckDefault">
    this.checkbox = document.createElement("input");
    this.checkbox.className = "form-check-input";
    this.checkbox.type = "checkbox";
    this.checkbox.value = "";
    this.checkbox.id = "check" + this.upperName;
    // this.checkbox.checked = true;

    this.checkbox.onclick = () => {
      this.showHide();
    };
    this.numObject = numObjects;
    numObjects++;

    //  i w class= iconClassName
    this.listIcon = document.createElement("i");
    this.listIcon.className = this.iconClassName;
    
    // <label class="form-check-label" for="checkIndeterminate">
    this.label = document.createElement("label");
    this.label.className = "form-check-label";
    this.label.htmlFor = "check" + this.upperName;
    this.label.innerHTML = this.upperName;
    
    this.parent.appendChild(this.checkbox);
    this.parent.appendChild(this.listIcon);
    this.parent.appendChild(this.label);

    controls.appendChild(this.parent);

    this.listIcon.style.marginLeft = "5px";
    this.label.style.marginLeft = "5px";
    this.parent.style.display = "flex";
  }

  addToPlane() {
    // creamos in div id= name '-coordinate-plane'
    this.plane = document.createElement("div");
    this.plane.id = this.name + "-coordinate-plane";
    this.plane.className = "objs";

    // agregamos un i class= iconClassName
    this.icon = document.createElement("i");
    this.icon.className = this.iconClassName;
    this.plane.appendChild(this.icon);

    
    // agregamos un div id= name '-coordinate-height'
    this.height = document.createElement("div");
    this.height.id = this.name + "-coordinate-height";
    this.height.className = "objs";

    // agregamos un i class= iconClassName
    this.heightIcon = document.createElement("i");
    this.heightIcon.className = this.iconClassName;
    this.height.appendChild(this.heightIcon);

    // agregamos el div a la pantalla
    plane.insertBefore(this.plane, plane.firstChild);
    height.insertBefore(this.height, height.firstChild);
  }

  showHide() {
    if (this.plane.style.display === "none") {
      this.plane.style.display = "flex";
      this.height.style.display = "flex";
    } else {
      this.plane.style.display = "none";
      this.height.style.display = "none";
    }
    
    Module.ccall(
      "showHideObj",
      null,
      ["number", "number", "number", "number"],
      [this.numObject, this.defectR.x, this.defectR.y, this.defectR.z]
    );
  }

  getPlaneCoordinates() {
    const DxN = D.cross(N);

    let h = this.R.dot(N);
    let xProj = this.R.dot(D);
    let yProj = this.R.dot(DxN);

    this.xPercent = 50 * (1 - xProj / 3);
    this.yPercent = 50 * (1 + yProj / 3);
    this.hPercent = 50 * (1 - h / 3);
  }

  updateR(x, y, z) {
    this.R.update(x, y, z);
    this.updatePlane();
  }

  updatePlane() {
    if (this.plane.style.display === "none") {
      return;
    }
    this.getPlaneCoordinates();

    this.plane.style.top = this.xPercent + "%";
    this.plane.style.left = `calc(${this.yPercent}% - 2px)`; 
    this.height.style.top = this.hPercent + "%";
  }
}

// snprintf(jsonInfo, sizeof(jsonInfo),
//              "const jsonInfo = {"
//              "    'velocity': %.3f,"
//              "    'time': %.3f,"
//              "    'RA': %.3f,"
//              "    'DEC': %.3f,"
//              "    'dirX': %.3f,"
//              "    'dirY': %.3f,"
//              "    'dirZ': %.3f,"
//              "    'normX': %.3f,"
//              "    'normY': %.3f,"
//              "    'normZ': %.3f,"
//              "    'diceX': %.3f,"
//              "    'diceY': %.3f,"
//              "    'diceZ': %.3f,"
//              "    'earthX': %.3f,"
//              "    'earthY': %.3f,"
//              "    'earthZ': %.3f,"
//              "    'saturnX': %.3f,"
//              "    'saturnY': %.3f,"
//              "    'saturnZ': %.3f"
//              "};"
//              "console.log(jsonInfo);",
//              velocity, t, atan2(dirY, dirX) * 180.0f / M_PI,
//              atan2(dirZ, sqrt(dirX * dirX + dirY * dirY)) * 180.0f / M_PI,
//              dirX, dirY, dirZ,
//              normX, normY, normZ,
//              objs[0].x_mean, objs[0].y_mean, objs[0].z_mean,
//              objs[1].x_mean, objs[1].y_mean, objs[1].z_mean,
//              objs[2].x_mean, objs[2].y_mean, objs[2].z_mean);
// char script[128];
//     sprintf(script, "document.getElementById('velocityDisplay').innerText = ' %.3f c';", velocity);
//     emscripten_run_script(script);

//     char script2[128];
//     sprintf(script2, "document.getElementById('timeDisplay').innerText = ' %.3f s';", t);
//     emscripten_run_script(script2);

//     char script3[128];
//     sprintf(script3, "document.getElementById('RADisplay').innerText = ' %.3f° ';", atan2(dirY, dirX) * 180.0f / M_PI);
//     emscripten_run_script(script3);

//     char script4[128];
//     sprintf(script4, "document.getElementById('DECDisplay').innerText = ' %.3f° ';", atan2(dirZ, sqrt(dirX * dirX + dirY * dirY)) * 180.0f / M_PI);
//     emscripten_run_script(script4);

//     char script5[256];
//     sprintf(script5, "D.update(%f, %f, %f); N.update(%f, %f, %f);", dirX, dirY, dirZ, normX, normY, normZ);
//     emscripten_run_script(script5);

//     sprintf(script5, "dice.updateR(%f, %f, %f);", objs[0].x_mean, objs[0].y_mean, objs[0].z_mean);
//     emscripten_run_script(script5);

//     sprintf(script5, "earth.updateR(%f, %f, %f);", objs[1].x_mean, objs[1].y_mean, objs[1].z_mean);
//     emscripten_run_script(script5);

//     sprintf(script5, "saturn.updateR(%f, %f, %f);", objs[2].x_mean, objs[2].y_mean, objs[2].z_mean);
//     emscripten_run_script(script5);
let jsonInfo = {
  velocity: 0,
  time: 0,
  RA: 0,
  DEC: 0,
  dirX: 0,
  dirY: 0,
  dirZ: 0,
  normX: 0,
  normY: 0,
  normZ: 0,
  diceX: 0,
  diceY: 0,
  diceZ: 0,
  earthX: 0,
  earthY: 0,
  earthZ: 0,
};

let display = {
  velocity: document.getElementById("velocityDisplay"),
  time: document.getElementById("timeDisplay"),
  RA: document.getElementById("RADisplay"),
  DEC: document.getElementById("DECDisplay"),
};

function execJsonInfo(){
  // Update the display elements with the values from jsonInfo
  display.velocity.innerText = jsonInfo.velocity.toFixed(3) + " c";
  display.time.innerText = jsonInfo.time.toFixed(3) + " s";
  display.RA.innerText = jsonInfo.RA.toFixed(3) + "° ";
  display.DEC.innerText = jsonInfo.DEC.toFixed(3) + "° ";

  D.update(jsonInfo.dirX, jsonInfo.dirY, jsonInfo.dirZ);
  N.update(jsonInfo.normX, jsonInfo.normY, jsonInfo.normZ);

  dice.updateR(jsonInfo.diceX, jsonInfo.diceY, jsonInfo.diceZ);
  earth.updateR(jsonInfo.earthX, jsonInfo.earthY, jsonInfo.earthZ);
  saturn.updateR(jsonInfo.saturnX, jsonInfo.saturnY, jsonInfo.saturnZ);
  dice.updatePlane();
  earth.updatePlane();
  saturn.updatePlane();
}


let D = new Vector3(1, 0, 0);
let N = new Vector3(0, 1, 0);

const dice = new objectManager("dice", "bi bi-dice-6-fill",new Vector3(5, 0, 1));
const earth = new objectManager("earth", "bi bi-globe-americas",new Vector3(1, 0, -0.30));
const saturn = new objectManager("saturn", "ph ph-planet",new Vector3(1, -0.1, 0.1));


dice.checkbox.click();
dice.updateR(1, 1, 2);