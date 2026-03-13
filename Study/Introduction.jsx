import { useState } from "react";

const project = {
  name: "RFID Access Control System",
  tagline: "Build an Arduino-based door lock with RFID auth + serial access logging",
  description: "An RC522 RFID reader checks cards/fobs against an allowlist stored in Arduino memory. Granted = green LED + relay clicks. Denied = red LED + buzzer. Every attempt is logged over Serial with a timestamp — your first taste of access control logs, replay attacks, and physical security.",
  tinkercad: "Prototype in TinkerCAD Circuits using Arduino Uno + simulated components, then transfer to physical build.",
  github: "Code goes on GitHub: Arduino .ino sketch + a README documenting the circuit, wiring diagram, and security notes.",
  cyber: "Ties to: Authentication (UID as credential), Access Logs (serial monitor), Replay Attacks (cloned RFID cards), Physical Security vs Digital Security."
};

const curriculum = [
  {
    week: "Week 1–2",
    title: "Arduino & Digital I/O",
    color: "#00ff9d",
    topics: [
      { name: "Arduino IDE setup + uploading sketches", done: true },
      { name: "pinMode, digitalWrite, digitalRead", done: true },
      { name: "Blink an LED (the hello world of circuits)", done: true },
      { name: "Button input with pull-up/pull-down resistors", done: false },
      { name: "Serial.begin() / Serial.println() — your first 'log'", done: true },
    ],
    project_step: "TinkerCAD: Build LED + button circuit. Button press logs 'ACCESS ATTEMPT' to Serial.",
    resources: ["Arduino official tutorials", "TinkerCAD Circuits (free, browser-based)"]
  },
  {
    week: "Week 3–4",
    title: "SPI Communication & the RC522 RFID Module",
    color: "#00cfff",
    topics: [
      { name: "What is SPI? (MOSI, MISO, SCK, SS pins)", done: true },
      { name: "How RFID works — EM fields, passive tags, UIDs", done: true },
      { name: "Install MFRC522 library in Arduino IDE", done: false },
      { name: "Read a card UID and print it to Serial", done: false },
      { name: "Store an allowed UID array in your sketch", done: false },
    ],
    project_step: "TinkerCAD: Simulate RFID scan (TinkerCAD has limited RFID sim — use button as stand-in, test logic). Physical: Read real card UIDs.",
    resources: ["MFRC522 Arduino library docs", "LastMinuteEngineers RC522 tutorial"]
  },
  {
    week: "Week 5",
    title: "Relay Control + Active Output",
    color: "#ffcc00",
    topics: [
      { name: "How a relay works (coil, NO/NC contacts)", done: false },
      { name: "Why you need a transistor or relay module (not direct from Arduino pin)", done: false },
      { name: "Flyback diode — protecting your circuit (you already know diodes!)", done: false },
      { name: "Wiring a 5V relay module to Arduino", done: false },
      { name: "Buzzer for denied access tone", done: false },
    ],
    project_step: "TinkerCAD: Wire relay module — trigger relay on button press. Add LED indicators (green/red).",
    resources: ["How Relays Work — All About Circuits", "TinkerCAD relay simulation"]
  },
  {
    week: "Week 6",
    title: "Putting It Together — Full Sketch",
    color: "#ff6b35",
    topics: [
      { name: "Combine RFID read + UID check + relay + LEDs + Serial log", done: false },
      { name: "Millis() for non-blocking timing (relay stays open 3 seconds)", done: false },
      { name: "Format your Serial log: timestamp (millis), UID, GRANTED/DENIED", done: false },
      { name: "Add a 'master card' that can add/remove UIDs at runtime", done: false },
    ],
    project_step: "Full working prototype in TinkerCAD (partial simulation) + physical breadboard build.",
    resources: []
  },
  {
    week: "Week 7",
    title: "GitHub + Documentation",
    color: "#b388ff",
    topics: [
      { name: "Git init, add, commit, push basics", done: false },
      { name: "Write a README.md: project overview, wiring diagram, components list", done: false },
      { name: "Comment your Arduino code professionally", done: false },
      { name: "Include a Fritzing or ASCII wiring diagram", done: false },
    ],
    project_step: "Push your .ino file + README to a public GitHub repo.",
    resources: ["GitHub Docs — Hello World", "Fritzing (free wiring diagram tool)"]
  },
  {
    week: "Bonus",
    title: "Cybersecurity Angles to Explore",
    color: "#ff4d6d",
    topics: [
      { name: "RFID Relay Attack — how attackers extend card range", done: false },
      { name: "UID Cloning — why UID-only auth is weak (Mifare Classic)", done: false },
      { name: "Encrypted auth — MIFARE's sector passwords (next level)", done: false },
      { name: "Physical vs logical security tradeoffs", done: false },
      { name: "Access log analysis — what makes a good log entry?", done: false },
    ],
    project_step: "Research only — no build needed. Great for your README's 'Security Notes' section.",
    resources: ["RFID Security — OWASP IoT", "Proxmark3 project (advanced)"]
  }
];

const components = [
  { name: "Arduino Uno", qty: "1", cost: "~$10" },
  { name: "RC522 RFID Module + cards/fobs", qty: "1 kit", cost: "~$5" },
  { name: "5V Relay Module", qty: "1", cost: "~$3" },
  { name: "LEDs (red + green)", qty: "2", cost: "<$1" },
  { name: "Buzzer (active)", qty: "1", cost: "<$1" },
  { name: "Resistors (220Ω, 10kΩ)", qty: "handful", cost: "<$1" },
  { name: "Breadboard + jumper wires", qty: "1 set", cost: "~$5" },
];

export default function App() {
  const [weeks, setWeeks] = useState(curriculum.map(w => ({ ...w, topics: w.topics.map(t => ({ ...t })) })));
  const [activeWeek, setActiveWeek] = useState(0);
  const [tab, setTab] = useState("curriculum");

  const toggleTopic = (wi, ti) => {
    setWeeks(prev => prev.map((w, i) => i !== wi ? w : {
      ...w, topics: w.topics.map((t, j) => j !== ti ? t : { ...t, done: !t.done })
    }));
  };

  const totalTopics = weeks.reduce((s, w) => s + w.topics.length, 0);
  const doneTopics = weeks.reduce((s, w) => s + w.topics.filter(t => t.done).length, 0);
  const pct = Math.round((doneTopics / totalTopics) * 100);

  const w = weeks[activeWeek];

  return (
    <div style={{
      minHeight: "100vh",
      background: "#0a0c10",
      color: "#e8eaf0",
      fontFamily: "'IBM Plex Mono', 'Courier New', monospace",
      padding: "0",
    }}>
      <style>{`
        @import url('https://fonts.googleapis.com/css2?family=IBM+Plex+Mono:wght@300;400;600;700&family=Syne:wght@700;800&display=swap');
        * { box-sizing: border-box; margin: 0; padding: 0; }
        ::-webkit-scrollbar { width: 4px; }
        ::-webkit-scrollbar-track { background: #0a0c10; }
        ::-webkit-scrollbar-thumb { background: #333; border-radius: 2px; }
        .topic-row:hover { background: rgba(255,255,255,0.04); }
        .week-btn:hover { filter: brightness(1.3); }
        .tab-btn:hover { background: rgba(255,255,255,0.07); }
        @keyframes scanline {
          0% { transform: translateY(-100%); }
          100% { transform: translateY(100vh); }
        }
        @keyframes pulse-dot {
          0%, 100% { opacity: 1; }
          50% { opacity: 0.2; }
        }
        @keyframes fade-in {
          from { opacity: 0; transform: translateY(8px); }
          to { opacity: 1; transform: translateY(0); }
        }
        .fade-in { animation: fade-in 0.3s ease forwards; }
      `}</style>

      {/* Scanline effect */}
      <div style={{
        position: "fixed", top: 0, left: 0, right: 0, bottom: 0,
        pointerEvents: "none", zIndex: 0, overflow: "hidden",
        background: "repeating-linear-gradient(0deg, transparent, transparent 2px, rgba(0,0,0,0.08) 2px, rgba(0,0,0,0.08) 4px)"
      }} />

      <div style={{ position: "relative", zIndex: 1, maxWidth: 900, margin: "0 auto", padding: "32px 20px" }}>

        {/* Header */}
        <div style={{ marginBottom: 40, animation: "fade-in 0.5s ease" }}>
          <div style={{ display: "flex", alignItems: "center", gap: 10, marginBottom: 8 }}>
            <div style={{ width: 8, height: 8, borderRadius: "50%", background: "#00ff9d", animation: "pulse-dot 1.5s infinite" }} />
            <span style={{ fontSize: 11, color: "#00ff9d", letterSpacing: 3, textTransform: "uppercase" }}>Arduino × Cybersecurity Project</span>
          </div>
          <h1 style={{ fontFamily: "'Syne', sans-serif", fontSize: "clamp(28px, 5vw, 48px)", fontWeight: 800, lineHeight: 1.1, letterSpacing: -1, color: "#fff", marginBottom: 12 }}>
            RFID Access<br />Control System
          </h1>
          <p style={{ color: "#888", fontSize: 13, lineHeight: 1.7, maxWidth: 560 }}>
            {project.description}
          </p>

          {/* Tags */}
          <div style={{ display: "flex", flexWrap: "wrap", gap: 8, marginTop: 16 }}>
            {["Arduino Uno", "RC522 RFID", "Relay Module", "Serial Logging", "TinkerCAD → Physical", "GitHub Upload"].map(tag => (
              <span key={tag} style={{ padding: "4px 10px", border: "1px solid #2a2d35", borderRadius: 4, fontSize: 11, color: "#aaa", letterSpacing: 1 }}>{tag}</span>
            ))}
          </div>
        </div>

        {/* Progress bar */}
        <div style={{ marginBottom: 32, padding: "16px 20px", border: "1px solid #1e2128", borderRadius: 8, background: "#0d0f14" }}>
          <div style={{ display: "flex", justifyContent: "space-between", alignItems: "center", marginBottom: 10 }}>
            <span style={{ fontSize: 11, color: "#666", letterSpacing: 2, textTransform: "uppercase" }}>Overall Progress</span>
            <span style={{ fontSize: 13, color: "#00ff9d", fontWeight: 600 }}>{doneTopics}/{totalTopics} topics · {pct}%</span>
          </div>
          <div style={{ height: 4, background: "#1a1d24", borderRadius: 2, overflow: "hidden" }}>
            <div style={{ height: "100%", width: `${pct}%`, background: "linear-gradient(90deg, #00ff9d, #00cfff)", borderRadius: 2, transition: "width 0.4s ease" }} />
          </div>
        </div>

        {/* Tabs */}
        <div style={{ display: "flex", gap: 4, marginBottom: 28, borderBottom: "1px solid #1e2128", paddingBottom: 0 }}>
          {["curriculum", "components", "project"].map(t => (
            <button key={t} className="tab-btn" onClick={() => setTab(t)} style={{
              padding: "8px 20px", background: "transparent", border: "none",
              borderBottom: tab === t ? "2px solid #00ff9d" : "2px solid transparent",
              color: tab === t ? "#00ff9d" : "#555", cursor: "pointer",
              fontFamily: "inherit", fontSize: 12, letterSpacing: 2, textTransform: "uppercase",
              transition: "all 0.2s", marginBottom: -1
            }}>{t}</button>
          ))}
        </div>

        {/* CURRICULUM TAB */}
        {tab === "curriculum" && (
          <div style={{ display: "grid", gridTemplateColumns: "200px 1fr", gap: 20, animation: "fade-in 0.3s ease" }}>
            {/* Week nav */}
            <div style={{ display: "flex", flexDirection: "column", gap: 4 }}>
              {weeks.map((wk, i) => {
                const done = wk.topics.filter(t => t.done).length;
                const total = wk.topics.length;
                return (
                  <button key={i} className="week-btn" onClick={() => setActiveWeek(i)} style={{
                    padding: "10px 14px", background: activeWeek === i ? `${wk.color}15` : "transparent",
                    border: `1px solid ${activeWeek === i ? wk.color : "#1e2128"}`,
                    borderRadius: 6, cursor: "pointer", fontFamily: "inherit", textAlign: "left",
                    transition: "all 0.2s"
                  }}>
                    <div style={{ fontSize: 10, color: wk.color, letterSpacing: 1, marginBottom: 2 }}>{wk.week}</div>
                    <div style={{ fontSize: 11, color: activeWeek === i ? "#fff" : "#666", lineHeight: 1.3 }}>{wk.title}</div>
                    <div style={{ fontSize: 10, color: "#444", marginTop: 4 }}>{done}/{total} done</div>
                  </button>
                );
              })}
            </div>

            {/* Week detail */}
            <div key={activeWeek} className="fade-in" style={{ padding: "20px 24px", border: `1px solid ${w.color}30`, borderRadius: 8, background: "#0d0f14" }}>
              <div style={{ fontSize: 11, color: w.color, letterSpacing: 3, textTransform: "uppercase", marginBottom: 6 }}>{w.week}</div>
              <h2 style={{ fontFamily: "'Syne', sans-serif", fontSize: 22, fontWeight: 800, color: "#fff", marginBottom: 20 }}>{w.title}</h2>

              <div style={{ display: "flex", flexDirection: "column", gap: 2, marginBottom: 24 }}>
                {w.topics.map((topic, ti) => (
                  <div key={ti} className="topic-row" onClick={() => toggleTopic(activeWeek, ti)} style={{
                    display: "flex", alignItems: "flex-start", gap: 12, padding: "10px 8px",
                    cursor: "pointer", borderRadius: 6, transition: "background 0.15s"
                  }}>
                    <div style={{
                      width: 18, height: 18, border: `1.5px solid ${topic.done ? w.color : "#333"}`,
                      borderRadius: 3, flexShrink: 0, marginTop: 1,
                      background: topic.done ? w.color : "transparent",
                      display: "flex", alignItems: "center", justifyContent: "center",
                      transition: "all 0.2s"
                    }}>
                      {topic.done && <span style={{ fontSize: 11, color: "#000", fontWeight: 700 }}>✓</span>}
                    </div>
                    <span style={{ fontSize: 13, color: topic.done ? "#666" : "#ccc", textDecoration: topic.done ? "line-through" : "none", lineHeight: 1.5 }}>
                      {topic.name}
                    </span>
                  </div>
                ))}
              </div>

              {/* Project step */}
              <div style={{ padding: "14px 16px", background: `${w.color}0d`, border: `1px solid ${w.color}30`, borderRadius: 6, marginBottom: w.resources.length ? 16 : 0 }}>
                <div style={{ fontSize: 10, color: w.color, letterSpacing: 2, marginBottom: 6 }}>▸ TINKERCAD / BUILD STEP</div>
                <p style={{ fontSize: 12, color: "#aaa", lineHeight: 1.6 }}>{w.project_step}</p>
              </div>

              {w.resources.length > 0 && (
                <div style={{ marginTop: 16 }}>
                  <div style={{ fontSize: 10, color: "#444", letterSpacing: 2, marginBottom: 8 }}>RESOURCES</div>
                  {w.resources.map((r, ri) => (
                    <div key={ri} style={{ fontSize: 12, color: "#555", padding: "4px 0", borderBottom: "1px solid #151820" }}>
                      → {r}
                    </div>
                  ))}
                </div>
              )}
            </div>
          </div>
        )}

        {/* COMPONENTS TAB */}
        {tab === "components" && (
          <div className="fade-in">
            <p style={{ color: "#666", fontSize: 12, marginBottom: 20 }}>Total estimated cost: ~$25–30. Everything available on Amazon or AliExpress.</p>
            <div style={{ display: "flex", flexDirection: "column", gap: 2 }}>
              {components.map((c, i) => (
                <div key={i} style={{ display: "grid", gridTemplateColumns: "1fr 80px 80px", gap: 16, padding: "14px 16px", borderBottom: "1px solid #131620", alignItems: "center" }}>
                  <span style={{ fontSize: 13, color: "#ccc" }}>{c.name}</span>
                  <span style={{ fontSize: 12, color: "#555", textAlign: "center" }}>×{c.qty}</span>
                  <span style={{ fontSize: 12, color: "#00ff9d", textAlign: "right" }}>{c.cost}</span>
                </div>
              ))}
            </div>
            <div style={{ marginTop: 28, padding: "16px 20px", border: "1px solid #1e2128", borderRadius: 8, background: "#0d0f14" }}>
              <div style={{ fontSize: 10, color: "#00cfff", letterSpacing: 2, marginBottom: 10 }}>▸ TINKERCAD SIMULATION NOTES</div>
              <p style={{ fontSize: 12, color: "#777", lineHeight: 1.7 }}>
                TinkerCAD Circuits has Arduino Uno, LEDs, buzzers, buttons, relays, and resistors built in. The RC522 RFID module isn't natively simulated — use a push button as a stand-in to test the access logic and Serial output. Once logic is verified, replicate on physical hardware.
              </p>
            </div>
          </div>
        )}

        {/* PROJECT TAB */}
        {tab === "project" && (
          <div className="fade-in" style={{ display: "flex", flexDirection: "column", gap: 16 }}>
            {[
              { label: "What it does", color: "#00ff9d", content: "Reads RFID card UIDs. Checks against hardcoded allowlist. Grants access (green LED + relay) or denies (red LED + buzzer). Logs every attempt to Serial with millis() timestamp and UID." },
              { label: "GitHub repo structure", color: "#00cfff", content: "rfid-access-control/\n├── rfid_lock.ino          ← main sketch\n├── README.md              ← wiring + instructions\n├── wiring_diagram.png     ← Fritzing export\n└── SECURITY_NOTES.md      ← RFID attack surface" },
              { label: "Cybersecurity angle", color: "#ffcc00", content: "Authentication: UID as a credential — what are the weaknesses?\nAccess Logs: Serial output is your first audit log. What info should every log entry contain?\nReplay Attacks: RFID cards can be cloned with ~$30 hardware. How would you defend against this?\nPhysical Security: The relay controls a physical lock — what happens if someone cuts power?" },
              { label: "Stretch goals", color: "#ff6b35", content: "• Add an LCD to display 'ACCESS GRANTED / DENIED'\n• Store UIDs in EEPROM so they survive power cycles\n• Add a keypad — two-factor auth (card + PIN)\n• Log to an SD card module instead of Serial\n• Connect to WiFi (ESP8266/ESP32 swap) and POST logs to a server" },
            ].map((item, i) => (
              <div key={i} style={{ padding: "18px 20px", border: `1px solid ${item.color}25`, borderRadius: 8, background: "#0d0f14" }}>
                <div style={{ fontSize: 10, color: item.color, letterSpacing: 3, textTransform: "uppercase", marginBottom: 10 }}>▸ {item.label}</div>
                <pre style={{ fontSize: 12, color: "#aaa", lineHeight: 1.8, whiteSpace: "pre-wrap", fontFamily: "inherit" }}>{item.content}</pre>
              </div>
            ))}
          </div>
        )}

      </div>
    </div>
  );
}