var template = document.createElement('template');

template.innerHTML = `
<style>
 :host { display: inline-block; }
 canvas { display: inline-block; }
</style>

<canvas id="removal-canvas" width=getWidth()> </canvas>
`

class RemoveObject extends HTMLElement {
  static get observedAttributes() {
    return ["protected", "destroy", "imgSrc"]
  }
  constructor() {
    super();
    this.attachShadow({mode: 'open'});

    this.shadowRoot.appendChild(template.content.cloneNode(true));
    this.img;
    this.protected = [];
    this.destroy = [];

//    const img = document.createElement('img');
//    img.id = "removal-img"
//    if (this.hasAttribute('imgSrc')) {
//      imgUrl = this.getAttribute('imgSrc');
//
//      console.log(imgUrl);
//
//      img.src = imgUrl;
//    }
//
//    this.shadowRoot.appendChild(img);
  }

  connectedCallback() { console.log("connectedCallback");
    const img = new Image();
    const imgSrc = this.getAttribute('imgSrc');
    img.src = imgSrc;

    const protectedRegions = this.getAttribute("protected");
    const destroyRegions = this.getAttribute("destroy");

    console.log("protectedRegions", protectedRegions)
    console.log("destroyRegions", destroyRegions)

    const canvas = this.shadowRoot.getElementById('removal-canvas');
    const ctx = canvas.getContext('2d');
    const shadow = this.shadowRoot;

    img.onload = function () {
      canvas.width = this.width;
      canvas.height = this.height;
      ctx.drawImage(img, 0, 0)
    }
    this.img = img;
  }

  redraw() {
    const canvas = this.shadowRoot.getElementById('removal-canvas');
    const ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    if (this.img) {
      ctx.drawImage(this.img, 0, 0)
    }

    this.protected.map(tri => this.drawTriangle(tri, "#008000"));


    this.destroy.map(tri => this.drawTriangle(tri, "#FF0000"));
//
//    destroyRegions.triangles.map(tri => {
//      drawTriangle(tri)
//    })
  }

  attributeChangedCallback(name, oldValue, newValue) {
    const parsedValues = JSON.parse(newValue);
    this[name] = parsedValues;

    this.redraw();
//    if (parsedValues) {
//      parsedValues.map(tri => this.drawTriangle(tri));
//    }


  }

  drawTriangle(points, color) {
    const ctx = this.shadowRoot.getElementById('removal-canvas').getContext('2d');

    ctx.fillStyle=color;
    const pointOne = points[0];
    const pointTwo = points[1];
    const pointThree = points[2];

    ctx.beginPath();
    ctx.moveTo(pointOne[0], pointOne[1]);
    ctx.lineTo(pointTwo[0], pointTwo[1]);
    ctx.lineTo(pointThree[0], pointThree[1]);
    ctx.fill();
  }
}

window.customElements.define('remove-object', RemoveObject);

/*

const template = document.createElement('template');
template.innerHTML = `
<style>
button {
  background: #1E88E5;
  color: white;
  padding: 2rem 4rem;
  border: 0;
  font-size: 1.5rem;
}
</style>
<button>Sup?</button>`;

class WhatsUp extends HTMLElement {

  constructor() {
    super();
    this.attachShadow({mode: 'open'});
    this.shadowRoot.appendChild(template.content.cloneNode(true));
    const button = this.shadowRoot.querySelector("button");
    button.addEventListener("click", this.handleClick);
  }

  handleClick(e) {
    alert("Sup?");
  }

}k

window.customElements.define('whats-up', WhatsUp);
*/
