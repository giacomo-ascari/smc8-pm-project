// https://www.youtube.com/watch?v=rGNUHigqUBM
// https://www.youtube.com/watch?v=iKAVRgIrUOU
// https://matthias-research.github.io/pages/tenMinutePhysics/index.html

// 1d simulation of incompressible and inviscid

// fluid simulation
// 1. modify velocity values (eg. add gravity)
// 2. make the fuild incompressible (projection)
// 3. move the velocity field (advection)
// repeat

class Velocity {
    constructor() {
        this.vx = 0;
        this.vy = 0;
    }
    sum(a) {
        this.vx += a.vx
        this.vy += a.vy
    }
    magnitude() {
        return Math.sqrt(
            Math.pow(this.vx, 2) + Math.pow(this.vy, 2)
        )
    }
}

class Cell {
    constructor() {
        this.velocity = new Velocity();
        this.pressure = 0;
        this.wall = false
    }
}

class Space {
    constructor(xSize, ySize, T) {
        // general info
        this.xSize = xSize;
        this.ySize = ySize;
        this.T = T
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

        let maxVelocity = 0

        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (!this.cells[x][y].wall) {
                if (Math.abs(this.cells[x][y].velocity.vx) > maxVelocity) {
                    maxVelocity = Math.abs(this.cells[x][y].velocity.vx)
                }
                if (Math.abs(this.cells[x][y].velocity.vy) > maxVelocity) {
                    maxVelocity = Math.abs(this.cells[x][y].velocity.vy)
                }
            }
        })

        iterate2d(this.xSize, this.ySize, (x, y, onBorder) => {
            if (this.cells[x][y].wall) {
                // walls get drawn as grey
                ctx.fillStyle = "#888888"
            } else {
                // pressure
                ctx.fillStyle = RGBtoString(getSciColor(this.cells[x][y].pressure * Math.random(), -10, 100))
                //ctx.fillStyle = RGBtoString(getSciColor(this.cells[x][y].velocity.magnitude(), 0, 1000))
            }

            ctx.fillRect(x * zoom, y * zoom, zoom, zoom);
            ctx.fillStyle = "#000000";
            ctx.fillText(this.cells[x][y].pressure, x * zoom + 5, y * zoom + 20);

            // velocity on x
            if (this.cells[x][y].velocity.vx > 0) {
                ctx.strokeStyle = "#00FF00";
            } else {
                ctx.strokeStyle = "#FF0000";
            }
            ctx.beginPath();
            ctx.arc(x * zoom + zoom/2, y * zoom, Math.abs(this.cells[x][y].velocity.vx) / maxVelocity * 15, 0, 2 * Math.PI);
            ctx.stroke();

            // velocity on y
            if (this.cells[x][y].velocity.vy > 0) {
                ctx.strokeStyle = "#00FF00";
            } else {
                ctx.strokeStyle = "#FF0000";
            }
            ctx.beginPath();
            ctx.arc(x * zoom, y * zoom + zoom/2, Math.abs(this.cells[x][y].velocity.vy) / maxVelocity * 15, 0, 2 * Math.PI);
            ctx.stroke();

        })
    }

    sim() {
        // add gravity
        /*iterate2d(this.xSize, this.ySize, (x, y) => {
            let gVelocity = new Velocity()
            gVelocity.vy = -9.81 * this.T
            if (!this.cells[x][y].wall) {
                this.cells[x][y].velocity.sum(gVelocity)
            }
        })*/
        this.cells[9][18].velocity.vx = -1
        this.cells[9][18].velocity.vy = -1

        // reset pressure
        iterate2d(this.xSize, this.ySize, (x, y) => {
            this.cells[x][y].pressure = 0
        })

        // projection (incompressible)
        for (let n = 0; n < 50; n++) {
            iterate2d(this.xSize, this.ySize, (x, y) => {

                let o = 1.9; // overrelaxation

                if (!this.cells[x][y].wall) {
                    
                    let divergence = 0;
                    let neighbors = 0;

                    // calculate divergence
                    if (!this.cells[x-1][y].wall) {
                        // cell to the left
                        divergence += this.cells[x-1][y].velocity.vx;
                        neighbors += 1
                    }
                    if (!this.cells[x][y-1].wall) {
                        // cell to the top
                        divergence += this.cells[x][y-1].velocity.vy;
                        neighbors += 1
                    }
                    if (!this.cells[x+1][y].wall) {
                        // cell to the right
                        divergence -= this.cells[x][y].velocity.vx;
                        neighbors += 1
                    }
                    if (!this.cells[x][y+1].wall) {
                        // cell to the bottom
                        divergence -= this.cells[x][y].velocity.vy;
                        neighbors += 1
                    }
                    divergence *= o

                    //console.log(`cell ${x} ${y}: divergence ${divergence} and neighbors ${neighbors}`)

                    // redistribute divergence
                    if (!this.cells[x-1][y].wall) {
                        // cell to the left
                        this.cells[x-1][y].velocity.vx -= divergence / neighbors;
                    }
                    if (!this.cells[x][y-1].wall) {
                        // cell to the top
                        this.cells[x][y-1].velocity.vy -= divergence / neighbors;
                    }
                    if (!this.cells[x+1][y].wall) {
                        // cell to the right
                        this.cells[x][y].velocity.vx += divergence / neighbors;
                    }
                    if (!this.cells[x][y+1].wall) {
                        // cell to the bottom
                        this.cells[x][y].velocity.vy += divergence / neighbors;
                    }

                    let density = 0.001
                    let gridspacing = 0.001
                    this.cells[x][y].pressure += (divergence / neighbors) * (density * gridspacing / this.T)
                    
                }
            })
        }

        // advection
        iterate2d(this.xSize, this.ySize, (x, y) => {
            if (!this.cells[x][y].wall) {

            }
        })
        
    }
}


function onload() {
    //const vw = Math.max(document.documentElement.clientWidth || 0, window.innerWidth || 0);
    //const vh = Math.max(document.documentElement.clientHeight || 0, window.innerHeight || 0);
    //console.log(vw.toString() + ", " + vh.toString());
    
    let space = new Space(12, 20, 0.1)
    console.log("space created")

    let zoom = 50
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