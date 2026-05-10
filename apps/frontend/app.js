const canvas = document.getElementById('screen');
const ctx = canvas.getContext('2d');

const pet = { x: 172, y: 320, dx: 0, dy: 0, xp: 0, hunger: 0 };
const control = { x: 0, y: 0 };

addEventListener('keydown', (e) => {
  if (e.key === 'ArrowRight') control.x = 1;
  if (e.key === 'ArrowLeft') control.x = -1;
  if (e.key === 'ArrowUp') control.y = -1;
  if (e.key === 'ArrowDown') control.y = 1;
  if (e.key === ' ') pet.xp += 50;
});

addEventListener('keyup', (e) => {
  if (e.key.startsWith('Arrow')) {
    control.x = 0;
    control.y = 0;
  }
});

function tick() {
  pet.dx += control.x * 0.5;
  pet.dy += control.y * 0.5;
  pet.x += pet.dx;
  pet.y += pet.dy;
  pet.dx *= 0.92;
  pet.dy *= 0.92;

  if (pet.x < 20 || pet.x > canvas.width - 84) pet.dx *= -0.8;
  if (pet.y < 40 || pet.y > canvas.height - 84) pet.dy *= -0.8;

  pet.hunger += 0.03;

  ctx.fillStyle = '#000';
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  ctx.fillStyle = '#8ef39e';
  ctx.fillRect(pet.x, pet.y, 64, 64);
  ctx.fillStyle = '#fff';
  ctx.font = '24px sans-serif';
  ctx.fillText(`XP: ${Math.floor(pet.xp)}`, 12, 30);
  ctx.fillText(`Hunger: ${Math.floor(pet.hunger)}`, 12, 60);

  requestAnimationFrame(tick);
}

tick();
