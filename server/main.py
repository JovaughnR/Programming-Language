import os
import subprocess
import tempfile
import uuid
import signal
from pathlib import Path

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

# ─── CONFIGURATION ────────────────────────────────────────────────────────────
# 🔧 Set LANG_BINARY to the path of your compiled language interpreter/compiler
# Examples:
#   LANG_BINARY = "/usr/local/bin/mylang"
#   LANG_BINARY = "./build/mylang"
LANG_BINARY: str = os.getenv("LANG_BINARY", "../lang/interpreter")

# 🔧 How your interpreter is invoked:
#   "file"  → mylang /tmp/code.lang
#   "stdin" → cat code.lang | mylang
#   "flag"  → mylang --run /tmp/code.lang
INVOCATION_MODE: str = os.getenv("INVOCATION_MODE", "file")   # "file" | "stdin" | "flag"
INVOCATION_FLAG: str = os.getenv("INVOCATION_FLAG", "--run")  # only used in "flag" mode

# 🔧 Safety limits
EXECUTION_TIMEOUT_S: int = int(os.getenv("EXEC_TIMEOUT", "10"))       # seconds
MAX_CODE_SIZE_BYTES: int = int(os.getenv("MAX_CODE_SIZE", "65536"))    # 64 KB
MAX_OUTPUT_SIZE_BYTES: int = int(os.getenv("MAX_OUTPUT", "524288"))    # 512 KB

ALLOWED_ORIGIN: str = os.getenv("ALLOWED_ORIGIN", "http://localhost:3000")
PORT: int = int(os.getenv("PORT", "4000"))
# ──────────────────────────────────────────────────────────────────────────────

app = FastAPI(title="LangIDE Backend", version="1.0.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# ─── Models ───────────────────────────────────────────────────────────────────

class RunRequest(BaseModel):
    code: str
    filename: str = "program.cx"
    stdin: str = ""       


class RunResponse(BaseModel):
    stdout: str
    stderr: str
    exitCode: int


# ─── Routes ───────────────────────────────────────────────────────────────────

@app.get("/health")
def health():
    binary_path = Path(LANG_BINARY)
    return {
        "status": "ok",
        "binary": LANG_BINARY,
        "binaryExists": binary_path.exists(),
        "mode": INVOCATION_MODE,
    }


@app.post("/api/run", response_model=RunResponse)
def run_code(req: RunRequest):
    code_bytes = req.code.encode("utf-8")
    if len(code_bytes) > MAX_CODE_SIZE_BYTES:
        raise HTTPException(status_code=400, detail=f"Code exceeds maximum size")

    if not Path(LANG_BINARY).exists():
        return RunResponse(
            stdout="",
            stderr=f"Binary not found at: {LANG_BINARY}",
            exitCode=1,
        )

    ext = Path(req.filename).suffix or ".lang"
    tmp_path = Path(tempfile.gettempdir()) / f"langide_{uuid.uuid4().hex}{ext}"

    try:
        tmp_path.write_text(req.code, encoding="utf-8")
        return _execute(tmp_path, req.code, req.stdin)   # ← pass stdin
    finally:
        try:
            tmp_path.unlink(missing_ok=True)
        except Exception:
            pass


# ─── Execution ────────────────────────────────────────────────────────────────

def _build_command(tmp_path: Path) -> tuple[list[str], bytes | None]:
    """Return (args, stdin_input) based on INVOCATION_MODE."""
    if INVOCATION_MODE == "stdin":
        return [LANG_BINARY], tmp_path.read_bytes()
    elif INVOCATION_MODE == "flag":
        return [LANG_BINARY, INVOCATION_FLAG, str(tmp_path)], None
    else:  # "file" (default)
        return [LANG_BINARY, str(tmp_path)], None


def _execute(tmp_path: Path, code: str, stdin_data: str = "") -> RunResponse:
    cmd, stdin_input = _build_command(tmp_path)

    # If mode is file/flag, stdin comes from the request
    if stdin_input is None:
        stdin_input = stdin_data.encode("utf-8") if stdin_data else b""

    try:
        result = subprocess.run(
            cmd,
            input=stdin_input,          # ← always pipe stdin, never None
            capture_output=True,
            timeout=EXECUTION_TIMEOUT_S,
            start_new_session=True,
        )

        stdout = result.stdout.decode("utf-8", errors="replace")
        stderr = result.stderr.decode("utf-8", errors="replace")

        if len(stdout.encode()) > MAX_OUTPUT_SIZE_BYTES:
            stdout = stdout[:MAX_OUTPUT_SIZE_BYTES] + "\n[Output truncated]"

        return RunResponse(stdout=stdout, stderr=stderr, exitCode=result.returncode)

    except subprocess.TimeoutExpired as e:
        # Python 3.10 fix — no .process attribute, use os.killpg differently
        try:
            import psutil
            for proc in psutil.process_iter():
                if proc.name() in [Path(LANG_BINARY).name]:
                    proc.kill()
        except Exception:
            pass

        stdout = (e.stdout or b"").decode("utf-8", errors="replace")
        stderr = (e.stderr or b"").decode("utf-8", errors="replace")
        stderr += f"\n[Execution timed out after {EXECUTION_TIMEOUT_S}s]"
        return RunResponse(stdout=stdout, stderr=stderr, exitCode=-1)

    except FileNotFoundError:
        return RunResponse(
            stdout="",
            stderr=f"[Process Error] Could not launch binary: {LANG_BINARY}",
            exitCode=-1,
        )
    except Exception as e:
        return RunResponse(stdout="", stderr=f"[Backend Error] {e}", exitCode=-1)


# ─── Entry point ──────────────────────────────────────────────────────────────

if __name__ == "__main__":
    import uvicorn

    print("\n╔═══════════════════════════════════════╗")
    print("║       LangIDE Backend Server          ║")
    print("╠═══════════════════════════════════════╣")
    print(f"║  Port:    {PORT:<28} ║")
    print(f"║  Binary:  {LANG_BINARY[:28]:<28} ║")
    print(f"║  Mode:    {INVOCATION_MODE:<28} ║")
    print(f"║  Timeout: {str(EXECUTION_TIMEOUT_S) + 's':<28} ║")
    print("╚═══════════════════════════════════════╝\n")

    if not Path(LANG_BINARY).exists():
        print(f"⚠  WARNING: Binary not found at \"{LANG_BINARY}\"")
        print(f"   Set LANG_BINARY env var to your interpreter path.\n")
    else:
        print(f"✓  Binary found: {LANG_BINARY}\n")

    uvicorn.run("main:app", host="0.0.0.0", port=PORT, reload=True)


# CMD: LANG_BINARY=../lang/cxlang python3 main.py