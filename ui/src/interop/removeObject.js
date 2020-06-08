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
    this.triangles = [];

    this.attachShadow({mode: 'open'});

    this.shadowRoot.appendChild(template.content.cloneNode(true));
    this.img;

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
      console.log(this.img)
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

    //ctx.save();
    //ctx.restore();

    this.triangles.map(tri => {
      drawTriangle(tri)
    })
  }

  attributeChangedCallback(name, oldValue, newValue) {
    console.log(newValue);
    const parsedValues = JSON.parse(newValue);

    this.redraw();
    if (parsedValues && parsedValues[0]) {
      parsedValues.map(tri => this.drawTriangle(tri));
    }


  }

  drawTriangle(points) {
    const ctx = this.shadowRoot.getElementById('removal-canvas').getContext('2d');

    const pointOne = points[0];
    const pointTwo = points[1];
    const pointThree = points[2];
    ctx.fillStyle = "#008000"

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
