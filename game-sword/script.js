let canvas = document.getElementById("tela")
let ctx = canvas.getContext("2d")
let LARGURA = 16, ALTURA = 32
const DOWN = 40, UP = 38, RIGHT = 39, LEFT = 37
let estado = 0
let walkx = 150
let walky = 0
let direction = 0

const imagem = new Image()
imagem.src = "gfx/character.png"

const draw = () => {
    ctx.clearRect(0, 0, 320, 320)
    ctx.drawImage(imagem,  // imagem a ser usada 
        estado * LARGURA,            // onde se inicia o corte na imagem  
        direction * ALTURA,                 // 
        LARGURA,           // largura do clip 
        ALTURA,            // altura do clip 
        walkx,             // posição x 
        walky,             // posição y
        LARGURA,
        ALTURA
    )
    estado++
    if (estado == 4)
        estado = 0

}

let runGame = setInterval(draw, 100)

document.addEventListener('keydown', (event) => {
    switch (event.keyCode) {
        case DOWN:
            direction = 0
            walky += 4
            break;
        case UP:
            direction = 2
            walky -= 4
            break;
        case RIGHT:
            direction = 1
            walkx += 4
            break;
        case LEFT:
            direction = 3
            walkx -= 4
    }
})