var template = document.createElement("template");

template.innerHTML = `
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
<style>
 :host { display: inline-block; }
 canvas { display: inline-block; }
 #container { position: relative; }
 #drawing-canvas {
    position: absolute;
    left: 0;
    top: 0;
    opacity: 1;
 }
 #img-canvas {
   position: aboslute;
   left: 0;
   top: 0;
 }

</style>

<div id="container">
  <canvas id="drawing-canvas"> </canvas>
  <canvas id="img-canvas"> </canvas>
</div>
<button class="btn btn-primary mt-3">Remove Highlighted Areas</button>
`;

class RemoveObject extends HTMLElement {
  static get observedAttributes() {
    return ["imgSrc", "markmode"];
  }

  constructor() {
    super();
    this.attachShadow({ mode: "open" });
    this.shadowRoot.appendChild(template.content.cloneNode(true));

    this.img;

    this.allowMouseMove = false;

    this.mousedownID = -1; //Global ID of mouse down interval

    this.brushSize = 10;
  }

  connectedCallback() {
    var imgCanvas = this.shadowRoot.getElementById("img-canvas");
    var drawingCanvas = this.shadowRoot.getElementById("drawing-canvas");
    var container = this.shadowRoot.getElementById("container");

    this.shadowRoot
      .querySelector("button")
      .addEventListener("click", (e) => this.handleSubmit(e));

    drawingCanvas.addEventListener("mousedown", (e) => this.handleMouseDown(e));
    drawingCanvas.addEventListener("mouseup", (e) => this.handleMouseUp(e));
    drawingCanvas.addEventListener("mouseout", (e) => this.handleMouseUp(e));
    drawingCanvas.addEventListener("mousemove", (e) => this.handleMouseMove(e));

    const img = new Image();
    img.src = this.getAttribute("imgSrc");
    img.onload = function () {
      container.setAttribute(
        "style",
        `width:${this.width}px;height:${this.height}px`
      );

      imgCanvas.width = this.width;
      imgCanvas.height = this.height;

      drawingCanvas.width = this.width;
      drawingCanvas.height = this.height;

      imgCanvas.getContext("2d").drawImage(img, 0, 0);
    };
  }

  handleSubmit(e) {
    var drawingCanvas = this.shadowRoot.getElementById("drawing-canvas");
    var ctx = drawingCanvas.getContext("2d");
    var imageData = ctx.getImageData(
      0,
      0,
      drawingCanvas.width,
      drawingCanvas.height
    );

    this.emitMarkings(getMarkings(imageData));
  }

  handleMouseDown(e) {
    var drawingCanvas = this.shadowRoot.getElementById("drawing-canvas");
    const drawingCtx = drawingCanvas.getContext("2d");

    this.drawSquare(e.offsetX, e.offsetY);

    const toggleAllowMouseMove = () => {
      this.allowMouseMove = !this.allowMouseMove;
    };

    if (this.mousedownID == -1)
      this.mousedownID = setInterval(toggleAllowMouseMove, 50);
  }

  handleMouseUp(e) {
    if (this.mousedownID != -1) {
      clearInterval(this.mousedownID);
      this.mousedownID = -1;
    }
  }

  handleMouseMove(e) {
    if (this.allowMouseMove) return;

    var drawingCanvas = this.shadowRoot.getElementById("drawing-canvas");
    if (this.mousedownID != -1) {
      const drawingCtx = drawingCanvas.getContext("2d");

      this.drawSquare(e.offsetX, e.offsetY);
    } else {
      console.log("mouse is moving");
    }
  }

  attributeChangedCallback(name, oldValue, newValue) {
    console.log("attributeChangedCallback", name, oldValue, newValue);
    this[name] = newValue;
  }

  emitMarkings(markings) {
    this.dispatchEvent(
      new CustomEvent("markings", {
        detail: markings,
        cancelable: false,
        bubbles: true,
        composed: true,
      })
    );
  }

  drawSquare(x, y) {
    const drawingCanvas = this.shadowRoot.getElementById("drawing-canvas");
    const drawingCtx = drawingCanvas.getContext("2d");

    var fillStyle = "";

    switch (this.getAttribute("markMode")) {
      case "destroy":
        fillStyle = "rgb(255,0,0,0.5)";
        break;
      case "protect":
        fillStyle = "rgb(0,255,0,0.5)";
        break;
      case "erase":
        fillStyle = "rgb(0, 0, 0, 0)";
        break;
      default:
        fillStyle = "rgb(0, 0, 0, 0)";
        break;
    }

    drawingCtx.fillStyle = fillStyle;
    drawingCtx.fillRect(x, y, this.brushSize, this.brushSize);
  }
}

function getMarkings(imageData) {
  var destroy = [];
  var protect = [];

  for (var idx = 0; idx < imageData.data.length; idx += 4) {
    var red = imageData.data[idx];
    var green = imageData.data[idx + 1];
    var alpha = imageData.data[idx + 3];

    //this match is wrong
    if (alpha === 255 && red) {
      destroy.push(1);
    } else {
      destroy.push(0);
    }
    if (alpha == 255 && green) {
      protect.push(1);
    } else {
      protect.push(0);
    }
  }

  return {
    destroy: destroy,
    protect: protect,
    imageWidth: imageData.width,
    imageHeight: imageData.height,
  };
}

window.customElements.define("remove-object", RemoveObject);
