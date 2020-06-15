var template = document.createElement('template');
/*
export interface RemoveObjectParams {
  imageName: string;
  showIntermediateSteps: boolean;
  lockRatio: boolean;
  onlyHorizontal: boolean;
  onlyVertical: boolean;
  markings: { destroy: number[]; protect: number[] };
  imageWidth: number;
  imageHeight: number;
}
 * */
template.innerHTML = `
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
  <button>Click Me</button>
</div>
`;

class RemoveObject extends HTMLElement {
  static get observedAttributes() {
    return ['protected', 'destroy', 'imgSrc'];
  }
  constructor() {
    super();
    this.attachShadow({ mode: 'open' });
    this.shadowRoot.appendChild(template.content.cloneNode(true));

    this.img;

    this.protected = [];
    this.destroy = [];
  }

  connectedCallback() {
    const img = new Image();
    const imgSrc = this.getAttribute('imgSrc');
    img.src = imgSrc;

    var canvas = this.shadowRoot.getElementById('img-canvas');
    var ctx = canvas.getContext('2d');

    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var container = this.shadowRoot.getElementById('container');

    const button = this.shadowRoot.querySelector('button');
    const foo = (e) => this.handleClick(e);
    button.addEventListener('click', foo);
    this.shadowRoot.addEventListener('click', function () {});

    img.onload = function () {
      container.setAttribute('style', `width:${this.width}px;height:${this.height}px`);

      canvas.width = this.width;
      canvas.height = this.height;
      drawingCanvas.width = this.width;
      drawingCanvas.height = this.height;

      ctx.drawImage(img, 0, 0);
    };
    this.img = img;
  }

  handleClick(e) {
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var ctx = drawingCanvas.getContext('2d');
    var imageData = ctx.getImageData(0, 0, drawingCanvas.width, drawingCanvas.height);
    var markings = getMarkings(imageData);
    this.emitMarkings(markings);
  }

  redraw() {
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var ctx = drawingCanvas.getContext('2d');
    ctx.clearRect(0, 0, drawingCanvas.width, drawingCanvas.height);

    ctx.strokeStyle = 'black';

    color = '#FF0000';
    ctx.fillStyle = color;
    this.destroy.map((tri) => this.drawTriangle(tri, '#FF0000'));

    var color = '#008000';
    ctx.fillStyle = color;
    this.protected.map((tri) => this.drawTriangle(tri, '#008000'));

    var imageData = ctx.getImageData(0, 0, drawingCanvas.width, drawingCanvas.height);
    var markings = getMarkings(imageData);
  }

  attributeChangedCallback(name, oldValue, newValue) {
    const parsedValues = JSON.parse(newValue);
    //if new triangle is added; redo the data
    this[name] = parsedValues;

    this.redraw();
  }

  drawTriangle(points, color) {
    const ctx = this.shadowRoot.getElementById('drawing-canvas').getContext('2d');
    ctx.stroke();

    const pointOne = points[0];
    const pointTwo = points[1];
    const pointThree = points[2];

    ctx.beginPath();
    ctx.moveTo(pointOne[0], pointOne[1]);
    ctx.lineTo(pointTwo[0], pointTwo[1]);
    ctx.lineTo(pointThree[0], pointThree[1]);
    ctx.fill();
  }

  emitMarkings(markings) {
    const onlyHorizontal = this.getAttribute('onlyHorizontal') === 'True';
    const onlyVertical = this.getAttribute('onlyVertical') === 'True';
    const lockRatio = this.getAttribute('lockRatio') === 'True';
    const showIntermediateSteps = this.getAttribute('showIntermediateSteps') === 'True';
    const imageName = this.getAttribute('imageName');

    var foo = {
      imageName: imageName,
      showIntermediateSteps,
      lockRatio,
      onlyHorizontal,
      onlyVertical,
      imageWidth: this.img.width,
      imageHeight: this.img.height,
      markings,
    };

    postData('http://localhost:3000/seam/remove-object/markings', foo).then((data) => {
      console.log(JSON.stringify(data));
    });
  }
}

//Maybe turn this into a two channel array?
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

  return { destroy: destroy, protect: protect };
}

// Example POST method implementation:
async function postData(url = '', data = {}) {
  // Default options are marked with *
  const response = await fetch(url, {
    method: 'POST',
    mode: 'cors',
    cache: 'no-cache',
    credentials: 'same-origin',
    headers: {
      'Content-Type': 'application/json',
    },
    redirect: 'follow',
    referrerPolicy: 'no-referrer',
    body: JSON.stringify(data),
  });
  return response.json();
}

async function getData(url = '', data = {}) {
  // Default options are marked with *
  const response = await fetch(url, {
    method: 'GET', // *GET, POST, PUT, DELETE, etc.
    mode: 'cors', // no-cors, *cors, same-origin
    cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
    credentials: 'same-origin', // include, *same-origin, omit
    headers: {
      'Content-Type': 'application/json',
      // 'Content-Type': 'application/x-www-form-urlencoded',
    },
    redirect: 'follow', // manual, *follow, error
    referrerPolicy: 'no-referrer', // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
  });
  return response.json(); // parses JSON response into native JavaScript objects
}

window.customElements.define('remove-object', RemoveObject);
