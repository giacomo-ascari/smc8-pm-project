// https://www.youtube.com/watch?v=rGNUHigqUBM
// https://www.youtube.com/watch?v=iKAVRgIrUOU
// https://matthias-research.github.io/pages/tenMinutePhysics/index.html
// https://web.archive.org/web/20160418004149/http://freespace.virgin.net/hugo.elias/graphics/x_water.htm
// https://www.youtube.com/watch?v=X0kjv0MozuY

class Cell {
    constructor() {
        this.pressureM1 = 0;
        this.pressureM2 = 0;
        this.pressure = 0
        this.material = 'a';
    }
}

let materialList = ["w", "a", "v", "m"]
let materialTable = {
    "w": { // wall
        factor: 0,
        simulations: 1
    },
    "a": { // air
        factor: 1,
        simulations: 1
    },
    "v": { // void
        factor: 0.97,
        simulations: 1
    },
    "m": { // metal
        factor: 1,
        simulations: 14
    }
}

class Space {
    constructor(xSize, ySize, T, border) {

        // general info
        this.xSize = xSize;
        this.ySize = ySize;
        this.T = T
        this.iteration = 0;
        this.border = border;

        // create cells
        this.cells = new Array(xSize)
        for (let x = 0; x < xSize; x++) {
            this.cells[x] = new Array(ySize)
            for (let y = 0; y < ySize; y++) {
                this.cells[x][y] = new Cell()
            }
        }

        // apply high dampening areas on the border
        iterate2d(xSize, ySize, (x, y) => {
            if (
                x >= 0 && x < border ||
                x >= xSize - border && x < xSize ||
                y >= 0 && y < border ||
                y >= ySize - border && y < ySize
                ) {
                this.cells[x][y].material = 'v';
            }
        })

        // medium for testing
        iterate2d(xSize, ySize, (x, y) => {
            if (x >= 170 && x < 190 && y >= 60 && y < 190) {
                this.cells[x][y].material = 'm';
            }
        })

        // just a wall for testing
        for (let x = 70; x < xSize -100; x++) {
            this.cells[x][100].material = 'w';
        }
    }

    render(canvas, zoom) {

        const ctx = canvas.getContext("2d");

        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (this.cells[x][y].material == 'w') {
                ctx.fillStyle = "#888888"
            } else {
                if (this.cells[x][y].material == 'v') {
                    ctx.fillStyle = RGBtoString(HSVtoRGB(0.7, 0.7, 0.5 * this.cells[x][y].pressure + 0.5))
                } else if (this.cells[x][y].material == 'm') {
                    ctx.fillStyle = RGBtoString(HSVtoRGB(0.95, 1, 0.5 * this.cells[x][y].pressure + 0.5))
                } else {
                    ctx.fillStyle = RGBtoString(HSVtoRGB(0.5, 1, 0.5 * this.cells[x][y].pressure + 0.5))
                    //ctx.fillStyle = RGBtoString(getSciColor(this.cells[x][y].pressure, -1, 1))
                }
                
            }

            ctx.fillRect(x * zoom, y * zoom, zoom, zoom);
            ctx.fillStyle = "#000000";
            // ctx.fillText(this.cells[x][y].pressure, x * zoom + 5, y * zoom + 20);

        })
    }

    adjacentAvgPressure(x, y) {
        let adjacent = 0;
        let adjacentPressure = 0;
        if (x != 0) {
            adjacentPressure += this.cells[x-1][y].pressureM1;
            adjacent++;
        }
        if (x != this.xSize-1) {
            adjacentPressure += this.cells[x+1][y].pressureM1;
            adjacent++;
        }
        if (y != 0) {
            adjacentPressure += this.cells[x][y-1].pressureM1;
            adjacent++;
        }
        if (y != this.ySize-1) {
            adjacentPressure += this.cells[x][y+1].pressureM1;
            adjacent++;
        }
        return adjacentPressure / adjacent;
    }

    simulatePressure(material) {
        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (this.cells[x][y].material == material) {
                this.cells[x][y].pressure = 2 * this.adjacentAvgPressure(x, y) - this.cells[x][y].pressureM2;
                this.cells[x][y].pressure *= materialTable[material].factor;
            }
        })
    }

    simulateTime(material) {
        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            this.cells[x][y].pressureM2 = this.cells[x][y].pressureM1;
            this.cells[x][y].pressureM1 = this.cells[x][y].pressure;
        })
    }

    sim() {

        // source
        for (let i = 0; i < this.xSize; i++) {
            //this.cells[i][41].pressureM1 = 1 * Math.sin(this.iteration * this.T * 1 * 2 * Math.PI);
        }
        this.cells[100][80].pressureM1 = 2 * Math.sin(this.iteration * this.T * 0.5 * 2 * Math.PI);
        this.cells[this.xSize-101][80].pressureM1 = 2 * Math.sin(this.iteration * this.T * 1 * 2 * Math.PI);

        // this cells pressure at sample n is:
        // p[n] = r * ( 2 * avg(p_left[n-1], p_right[n-1], p_top[n-1], p_bot[n-1]) - p[n-2] )
        // where r is dampening

        // sim
        let simsCount = {}
        let minSims = 10000;
        let maxSims = 0;

        materialList.forEach((item)=>{
            simsCount.item = 0
            if (materialTable)
        })
        this.simulatePressure("a");
        this.simulatePressure("w");
        this.simulatePressure("v");
        this.simulatePressure("m");
        this.simulateTime();


        this.iteration++;

    }
}


function onload() {
    //const vw = Math.max(document.documentElement.clientWidth || 0, window.innerWidth || 0);
    //const vh = Math.max(document.documentElement.clientHeight || 0, window.innerHeight || 0);
    //console.log(vw.toString() + ", " + vh.toString());
    
    // if fs is 192kHz then a cell is wide 1.786mm
    // v = 343 m/s --> h = v / fs
    let space = new Space(280, 280, 0.1, 40)
    console.log("space created")

    let zoom = 3
    const canvas = document.getElementById("myCanvas");
    canvas.width = space.xSize * zoom;
    canvas.height = space.ySize * zoom;
    
    setInterval(() => {
        space.sim()
        space.render(canvas, zoom)
    }, 50)
    console.log("interval set")

}