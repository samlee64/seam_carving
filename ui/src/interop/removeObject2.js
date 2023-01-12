var template = document.createElement('template');

// I am going to create an image and you will be manually drawing the mask with a cursor
//
//
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

class RemoveObject2 extends HTMLElement {
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

    this.destroyPoints = [];
    this.protectedPoints = [];
    this.addEventListener('response', console.log);
  }

  connectedCallback() {
    var canvas = this.shadowRoot.getElementById('img-canvas');
    var ctx = canvas.getContext('2d');
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var container = this.shadowRoot.getElementById('container');

    const el = (e) => this.handleClick(e);
    this.shadowRoot.querySelector('button').addEventListener('click', el);

    const la = (e) => this.drawingCanvasClick(e);
    drawingCanvas.addEventListener('click', la);

    const img = new Image();
    img.src = this.getAttribute('imgSrc');
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

  drawingCanvasClick(e) {
    this.dispatchEvent(new CustomEvent('drawing-click'));
  }

  handleClick(e) {
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var ctx = drawingCanvas.getContext('2d');
    var imageData = ctx.getImageData(0, 0, drawingCanvas.width, drawingCanvas.height);

    this.emitMarkings(getMarkings(imageData));
  }

  redraw() {
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var ctx = drawingCanvas.getContext('2d');

    ctx.clearRect(0, 0, drawingCanvas.width, drawingCanvas.height);
    ctx.strokeStyle = 'black';

    ctx.fillStyle = 'rgb(255,0,0,0.5)';
    this.destroy.map((pointRadius) => this.drawMaskValues(pointRadius, '#FF0000'));

    ctx.fillStyle = 'rgb(0,255,0, 0.5)';
    this.protected.map((pointRadius) => this.drawMaskValues(pointRadius, '#FF0000'));
  }

  attributeChangedCallback(name, oldValue, newValue) {
    const parsedValues = JSON.parse(newValue);
    this[name] = parsedValues;

    this.redraw();
  }

  getImageCanvas() {
    return this.shadowRoot.getElementById('img-canvas');
  }

  getDrawingCanvas() {
    return this.shadowRoot.getElementById('drawing-canvas');
  }

  drawMaskValues(pointRadius) {
    const { point, radius } = pointRadius;

    const canvas = this.getDrawingCanvas();
    const ctx = canvas.getContext('2d');

    const startX = Math.max(point[0] - radius, 0);
    const startY = Math.max(point[1] - radius, 0);
    const endX = Math.min(point[0] + radius, canvas.width);
    const endY = Math.min(point[1] + radius, canvas.height);

    ctx.fillStyle = 'red';
    ctx.fillRect(startX, startY, radius, radius);
  }

  // how do I want to store the data?
  // I can store it as a series of point + radius
  // yeah something like that.
  // and then calculate total area afterwards?
  // or is it better to just store the total image
  // lets do both
  // // first lets do series of point+ radius
  // // second lets do total image

  // gets params and sends information to api
  emitMarkings(markings) {
    console.log('emitMarkings');
    const onlyHorizontal = this.getAttribute('onlyHorizontal') === 'True';
    const onlyVertical = this.getAttribute('onlyVertical') === 'True';
    const lockRatio = this.getAttribute('lockRatio') === 'True';
    const showIntermediateSteps = this.getAttribute('showIntermediateSteps') === 'True';
    const imageName = this.getAttribute('imageName');

    var params = {
      imageName: imageName,
      showIntermediateSteps,
      lockRatio,
      onlyHorizontal,
      onlyVertical,
      imageWidth: this.img.width,
      imageHeight: this.img.height,
      markings,
    };

    postData('http://localhost:3000/seam/remove-object/markings', params).then((data) => {
      console.log(JSON.stringify(data));
      this.emitResponse(data);
    });
  }

  emitResponse(data) {
    console.log('emit response');
    this.dispatchEvent(new CustomEvent('response', { detail: data }));
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

async function postData(url = '', data = {}) {
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

window.customElements.define('remove-object-2', RemoveObject2);
