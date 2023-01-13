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
    return ['imgSrc', 'markmode'];
  }

  constructor() {
    super();
    this.attachShadow({ mode: 'open' });
    this.shadowRoot.appendChild(template.content.cloneNode(true));

    this.img;

    this.mousedownID = -1; //Global ID of mouse down interval
  }

  connectedCallback() {
    var canvas = this.shadowRoot.getElementById('img-canvas');
    var ctx = canvas.getContext('2d');
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var container = this.shadowRoot.getElementById('container');

    const el = (e) => this.handleSubmit(e);
    this.shadowRoot.querySelector('button').addEventListener('click', el);

    drawingCanvas.addEventListener('mousedown', (e) => this.handleMouseDown(e));
    drawingCanvas.addEventListener('mouseup', (e) => this.handleMouseUp(e));
    drawingCanvas.addEventListener('mouseout', (e) => this.handleMouseUp(e));

    drawingCanvas.addEventListener('mousemove', (e) => this.handleMouseMove(e));

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

  handleSubmit(e) {
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    var ctx = drawingCanvas.getContext('2d');
    var imageData = ctx.getImageData(0, 0, drawingCanvas.width, drawingCanvas.height);

    this.emitResponse('asdfkadfsadfs');
    this.emitMarkings(getMarkings(imageData));
  }

  handleMouseDown(e) {
    function whilemousedown() {
      console.log('mouse is being dragged');
    }

    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    const drawingCtx = drawingCanvas.getContext('2d');
    const fillStyle = this.getAttribute('markMode') == 'destroy' ? 'rgb(255,0,0,0.5)' : 'rgb(0, 255, 0, 0.5)';
    console.log('fillStyle', fillStyle);
    drawingCtx.fillStyle = fillStyle;
    drawingCtx.fillRect(e.offsetX, e.offsetY, 10, 10);
    if (this.mousedownID == -1)
      //Prevent multimple loops!
      this.mousedownID = setInterval(whilemousedown, 100 /*execute every 100ms*/);
  }

  handleMouseUp(e) {
    if (this.mousedownID != -1) {
      //Only stop if exists
      clearInterval(this.mousedownID);
      this.mousedownID = -1;
    }
  }

  handleMouseMove(e) {
    var drawingCanvas = this.shadowRoot.getElementById('drawing-canvas');
    if (this.mousedownID != -1) {
      const drawingCtx = drawingCanvas.getContext('2d');

      // depending on what is selected
      const fillStyle =
        this.getAttribute('markMode') == 'destroy' ? 'rgb(255,0,0,0.5)' : 'rgb(0, 255, 0, 0.5)';
      console.log('fillStyle', fillStyle);
      drawingCtx.fillStyle = fillStyle;
      drawingCtx.fillRect(e.offsetX, e.offsetY, 10, 10);
    } else {
      console.log('mouse is moving');
    }
  }

  attributeChangedCallback(name, oldValue, newValue) {
    console.log('attributeChangedCallback', name, oldValue, newValue);
    this[name] = newValue;
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
  //emitMarkings(markings) {
  //  console.log('emitMarkings', markings);
  //  const onlyHorizontal = this.getAttribute('onlyHorizontal') === 'True';
  //  const onlyVertical = this.getAttribute('onlyVertical') === 'True';
  //  const lockRatio = this.getAttribute('lockRatio') === 'True';
  //  const showIntermediateSteps = this.getAttribute('showIntermediateSteps') === 'True';
  //  const imageName = this.getAttribute('imageName');

  //  var params = {
  //    imageName: imageName,
  //    showIntermediateSteps,
  //    lockRatio,
  //    onlyHorizontal,
  //    onlyVertical,
  //    imageWidth: this.img.width,
  //    imageHeight: this.img.height,
  //    markings,
  //  };

  //  postData('http://localhost:3000/seam/remove-object/markings', params).then((data) => {
  //    console.log(JSON.stringify(data));
  //    this.emitResponse(data);
  //  });
  //}

  emitResponse(data) {
    console.log('emitResponse');
    this.dispatchEvent(
      new CustomEvent('response', { detail: data, cancelable: false, bubbles: true, composed: true }),
    );
  }

  emitMarkings(markings) {
    console.log('emitMarkings', markings);

    this.dispatchEvent(
      new CustomEvent('markings', { detail: markings, cancelable: false, bubbles: true, composed: true }),
    );
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
