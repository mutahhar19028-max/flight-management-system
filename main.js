const { app, BrowserWindow, ipcMain } = require("electron");
const path = require("path");
const { spawn } = require("child_process");
const fs = require("fs");
const sqlite3 = require("sqlite3").verbose();

let mainWindow;
let cppBackend;
let responseQueue = new Map();

const logFilePath = path.join(__dirname, "frontend_log.txt");

function log(msg) {
  const timestamp = new Date().toISOString();
  fs.appendFileSync(logFilePath, `[${timestamp}] ${msg}\n`);
  console.log(msg);
}

function launchCppBackend() {
  const cppPath = path.join(__dirname, "build","flightSystemJSON");
  
  log("Launching C++ backend: " + cppPath);
  
  try {
    cppBackend = spawn(cppPath);
    
    cppBackend.stdout.on("data", (data) => {
      const output = data.toString().trim();
      log(`C++ stdout: ${output}`);
      
      try {
        const jsonResponse = JSON.parse(output);
        const actionId = jsonResponse.actionId;
        
        if (actionId && responseQueue.has(actionId)) {
          const resolver = responseQueue.get(actionId);
          resolver(jsonResponse);
          responseQueue.delete(actionId);
        }
      } catch (e) {
        log(`Non-JSON output from C++: ${output}`);
      }
    });
    
    cppBackend.stderr.on("data", (data) => {
      log(`C++ stderr: ${data.toString()}`);
    });
    
    cppBackend.on("close", (code) => {
      log(`C++ backend exited with code ${code}`);
    });
    
    log("C++ backend launched successfully");
  } catch (err) {
    log(`Failed to launch C++ backend: ${err.message}`);
  }
}

ipcMain.handle("cpp-action", async (event, cmd) => {
  return new Promise((resolve, reject) => {
    if (!cppBackend || cppBackend.killed) {
      return reject({ success: false, message: "C++ backend not running" });
    }
    
    const actionId = `action-${Date.now()}-${Math.random().toString(36).substr(2, 9)}`;
    cmd.actionId = actionId;
    
    responseQueue.set(actionId, resolve);
    
    setTimeout(() => {
      if (responseQueue.has(actionId)) {
        responseQueue.delete(actionId);
        reject({ success: false, message: "Backend timeout" });
      }
    }, 10000);
    
    try {
      cppBackend.stdin.write(JSON.stringify(cmd) + "\n");
      log(`Sent to C++: ${JSON.stringify(cmd)}`);
    } catch (err) {
      responseQueue.delete(actionId);
      reject({ success: false, message: err.message });
    }
  });
});

ipcMain.handle("get-all-tables", async () => {
  return new Promise((resolve, reject) => {
    const dbPath = path.join(__dirname, "data", "system-data.db");
    log(`Opening database: ${dbPath}`);
    
    const db = new sqlite3.Database(dbPath, sqlite3.OPEN_READONLY, (err) => {
      if (err) {
        log(`DB open error: ${err.message}`);
        return reject({ error: err.message });
      }
    });
    
    const tables = {};
    const tableNames = ["customers", "flights", "tickets", "planes", "airports", "offers"];
    let completed = 0;
    
    tableNames.forEach(tableName => {
      db.all(`SELECT * FROM ${tableName}`, [], (err, rows) => {
        if (err) {
          log(`Query error for ${tableName}: ${err.message}`);
          tables[tableName] = [];
        } else {
          tables[tableName] = rows;
          log(`Fetched ${rows.length} rows from ${tableName}`);
        }
        
        completed++;
        if (completed === tableNames.length) {
          db.close();
          resolve(tables);
        }
      });
    });
  });
});

ipcMain.handle("show-database", () => {
  if (mainWindow) {
    mainWindow.loadFile(path.join(__dirname, "frontend", "database.html"));
    log("Navigated to database.html");
  }
});

ipcMain.handle("go-back", () => {
  if (mainWindow) {
    mainWindow.loadFile(path.join(__dirname, "frontend", "index.html"));
    log("Navigated to index.html");
  }
});

ipcMain.handle("log-action", (event, msg) => {
  log(msg);
});

ipcMain.handle("get-customer-id", () => {
  const customerId = Math.floor(Math.random() * 9000) + 1000;
  log(`Generated customer ID: ${customerId}`);
  return customerId;
});

app.whenReady().then(() => {
  mainWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    webPreferences: {
      preload: path.join(__dirname, "frontend", "preload.js"),
      contextIsolation: true,
      nodeIntegration: false,
    },
  });
  
  mainWindow.loadFile(path.join(__dirname, "frontend", "index.html"));
  log("Main window created");
  
  launchCppBackend();
});

app.on("window-all-closed", () => {
  if (cppBackend && !cppBackend.killed) {
    cppBackend.kill();
    log("C++ backend killed");
  }
  app.quit();
});