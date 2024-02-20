// https://www.youtube.com/watch?v=rGNUHigqUBM
// https://www.youtube.com/watch?v=iKAVRgIrUOU
// https://matthias-research.github.io/pages/tenMinutePhysics/index.html

// 1d simulation of incompressible and inviscid

class Cell {
    constructor() {
        this.pressure = 0;
        this.newPressure = 0;
        this.wall = false
    }
}

class Space {
    constructor(xSize, ySize, T) {
        // general info
        this.xSize = xSize;
        this.ySize = ySize;
        this.T = T
        this.iteration = 0;
        // create cell space with border
        this.cells = new Array(xSize)
        for (let x = 0; x < xSize; x++) {
            this.cells[x] = new Array(ySize)
            for (let y = 0; y < ySize; y++) {
                // if on border, it's a wall
                this.cells[x][y] = new Cell()
                this.cells[x][y].wall = x == 0 || y == 0 || x == xSize-1 || y == ySize-1
                
            }
        }
    }

    render(canvas, zoom) {

        const ctx = canvas.getContext("2d");

        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (this.cells[x][y].wall) {
                // walls get drawn as grey
                ctx.fillStyle = "#888888"
            } else {
                // pressure
                ctx.fillStyle = RGBtoString(getSciColor(this.cells[x][y].pressure, -10, 100))
            }

            ctx.fillRect(x * zoom, y * zoom, zoom, zoom);
            ctx.fillStyle = "#000000";
            ctx.fillText(this.cells[x][y].pressure, x * zoom + 5, y * zoom + 20);

        })
    }

    sim() {

        if (this.iteration == 0)
            this.cells[5][10].pressure = 1;

        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (!onBorder) {
                this.cells[x][y].newPressure =
                    (this.cells[x+1][y].pressure +
                    this.cells[x-1][y].pressure +
                    this.cells[x][y+1].pressure +
                    this.cells[x][y-1].pressure) / 4 - this.cells[x][y].pressure;
            }
        })

        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (!onBorder) {
                this.cells[x][y].pressure = this.cells[x][y].newPressure;
            }
        })


        this.iteration++;

    }
}


function onload() {
    //const vw = Math.max(document.documentElement.clientWidth || 0, window.innerWidth || 0);
    //const vh = Math.max(document.documentElement.clientHeight || 0, window.innerHeight || 0);
    //console.log(vw.toString() + ", " + vh.toString());
    
    let space = new Space(12, 20, 0.05)
    console.log("space created")

    let zoom = 30
    const canvas = document.getElementById("myCanvas");
    canvas.width = space.xSize * zoom;
    canvas.height = space.ySize * zoom;
    
    setInterval(() => {
        space.sim()
        space.render(canvas, zoom)
        console.log("frame")
    }, 1000)
    console.log("interval set")

}