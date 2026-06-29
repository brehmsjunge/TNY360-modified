const { app, BrowserWindow } = require('electron')
const path = require('path')
const http = require('http')
const fs = require('fs')

const isDev = !app.isPackaged
const WEBPORTAL_DIR = isDev
  ? path.join(__dirname, '../Firmware/extras/WebPortal/.output/public')
  : path.join(process.resourcesPath, 'webportal')

const PORT = 3333

function startServer() {
  return new Promise((resolve) => {
    const server = http.createServer((req, res) => {
      let filePath = path.join(WEBPORTAL_DIR, req.url === '/' ? '/index.html' : req.url)

      if (!fs.existsSync(filePath)) {
        filePath = path.join(WEBPORTAL_DIR, 'index.html')
      }

      const ext = path.extname(filePath)
      const mimeTypes = {
        '.html': 'text/html',
        '.js':   'application/javascript',
        '.css':  'text/css',
        '.ico':  'image/x-icon',
        '.glb':  'model/gltf-binary',
        '.json': 'application/json',
      }

      res.setHeader('Content-Type', mimeTypes[ext] || 'application/octet-stream')
      fs.createReadStream(filePath).pipe(res)
    })

    server.listen(PORT, '127.0.0.1', () => resolve(server))
  })
}

async function createWindow() {
  await startServer()

  const win = new BrowserWindow({
    width: 1280,
    height: 800,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
    }
  })

  win.loadURL(`http://127.0.0.1:${PORT}`)

  win.webContents.on('did-finish-load', () => {
    win.webContents.executeJavaScript(`
      if (!localStorage.getItem('fallback_ip')) {
        localStorage.setItem('fallback_ip', '192.168.4.1');
        window.location.reload();
      }
    `)
  })
}

app.whenReady().then(createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit()
})