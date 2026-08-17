<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=32&duration=3000&pause=1000&color=00FF00&center=true&vCenter=true&width=600&lines=SYSTEM+UNLOCKED;LOAD+ANY+.SYS;NO+SIGNATURE+REQUIRED;NO+TEST+MODE;KERNEL+IS+YOURS" alt="Typing SVG" />
</p>

---

<p align="center">
  <img src="https://img.shields.io/badge/Windows-10%20%7C%2011-00FF00?style=for-the-badge&logo=windows&logoColor=white&color=00FF00"/>
  <img src="https://img.shields.io/badge/Architecture-x64-00FF00?style=for-the-badge&logo=amd&logoColor=white&color=00FF00"/>
  <img src="https://img.shields.io/badge/License-MIT-00FF00?style=for-the-badge&logo=opensource&logoColor=white&color=00FF00"/>
  <img src="https://img.shields.io/badge/Status-Active-00FF00?style=for-the-badge&logo=statuspal&logoColor=white&color=00FF00"/>
  <img src="https://img.shields.io/badge/Version-2.0-00FF00?style=for-the-badge&logo=github&logoColor=white&color=00FF00"/>
  <img src="https://img.shields.io/badge/PRs-Welcome-00FF00?style=for-the-badge&logo=github&logoColor=white&color=00FF00"/>
</p>

---

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=00FF00&height=200&section=header&text=SysUnlocked&fontSize=80&fontColor=000000&animation=fadeIn" alt="Header" />
</p>

---

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=24&duration=4000&pause=500&color=00FF00&center=true&vCenter=true&width=800&lines=UNLOCK+THE+KERNEL;LOAD+ANY+.SYS;NO+SIGNATURE+REQUIRED;NO+TEST+MODE;NO+LIMITS" alt="Typing SVG" />
</p>

---

⚡ SysUnlocked

UNLOCK THE KERNEL. LOAD ANY .SYS. NO SIGNATURE. NO LIMITS.

SysUnlocked is a next-generation Windows kernel driver loader that completely bypasses Microsoft's signature enforcement. Load any unsigned .sys driver instantly, without Test Mode, without BCDEdit, without rebooting, and without leaving traces.

---

🔥 Why SysUnlocked?

Microsoft says:

"You can't load unsigned drivers."
"You need Test Mode."
"You need to disable Secure Boot."
"You need to buy an EV certificate."
"You need to restart your system."

SysUnlocked says:

NO.

Requirement Normal Way SysUnlocked
Signed Driver ✅ Required ❌ Bypassed
Test Mode ✅ Required ❌ Bypassed
BCDEdit ✅ Required ❌ Bypassed
Reboot ✅ Required ❌ Bypassed
Secure Boot Off ✅ Required ❌ Bypassed
EV Certificate ✅ Required ❌ Bypassed
Admin Rights ✅ Required ✅ Required
Time Hours Seconds

---

🧠 How SysUnlocked Works

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=20&duration=3000&pause=1000&color=00FF00&center=true&vCenter=true&width=600&lines=BYPASSING+DSE...;DISABLING+PATCHGUARD...;LOADING+INTO+KERNEL...;DONE.+DRIVER+RUNNING!" alt="Bypass Animation" />
</p>

SysUnlocked combines 6 advanced kernel techniques to load your driver:

1️⃣ Memory Loading

· Driver is loaded directly into kernel memory — never touches the disk
· No file creation, no traces, no detection

2️⃣ DSE Bypass

· Runtime patch of CiValidateImageHeader in ci.dll
· Microsoft's signature check is disabled in memory

3️⃣ PatchGuard Disable

· Disables KiInitializePatchGuard in ntoskrnl.exe
· Windows' anti-tampering protection is neutralized

4️⃣ HVCI Bypass

· Bypasses Hypervisor-protected Code Integrity
· Works even with virtualization-based security enabled

5️⃣ BYOVD Fallback

· Uses signed vulnerable drivers (e.g., RTCore64.sys, gdrv.sys)
· Injects your payload via DeviceIoControl

6️⃣ Self-Destruct

· Driver file is deleted immediately after loading
· No evidence remains on the system

---

🛡️ Protections Bypassed

<p align="center">
  <img src="https://img.shields.io/badge/DSE-BYPASSED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/PatchGuard-DISABLED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/HVCI-BYPASSED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Secure%20Boot-UNTOUCHED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/SMEP%2FSMAP-BYPASSED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Test%20Mode-NOT%20REQUIRED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Windows%20Defender-NO%20FILE%20WRITTEN-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Event%20Logging-MINIMAL%20TRACES-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
</p>

---

📦 Features

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=shark&color=00FF00&height=100&section=footer&text=FEATURES&fontSize=40&fontColor=000000" alt="Features" />
</p>

Feature Description
✅ EXE Mode Full GUI — browse and load any .sys
✅ DLL Mode Silent loading via sys.ini
✅ No Disk Write Driver stays in memory only
✅ Self-Destruct Driver file deleted after loading
✅ No Traces Minimal event logging
✅ Windows 10/11 x64 Fully supported
✅ BYOVD Ready Falls back to signed vulnerable drivers
✅ Load & Unload Full control via GUI
✅ One-Click Load Load driver in seconds

---

📁 Project Structure

EXE Mode (Full GUI):

```
      ├── SysUnlocked.exe         # exe
      └── Loader.sys              # Your driver (copied during build)

```

DLL Mode (Silent Loader):

```
     ├── SysUnlocked.dll         # Silent loader version
     ├── sys.ini                 # Configuration file
     └── MyDriver.sys            # Your driver (placed manually)
```

---

📄 sys.ini Configuration

For DLL Mode, create sys.ini in the same folder as SysUnlocked.dll:

```ini
[Settings]
Driver=yourDriver.sys
```

Replace yourDriver.sys with the actual name of your driver file. The DLL will read this file and load the specified driver automatically.

---

🎯 How to Use

🔹 EXE Mode (Full GUI)

1. Run SysUnlocked.exe as Administrator
2. Click Browse → select your .sys file
3. Click Load Sys → driver loads in 2-5 seconds
4. Click Unload Sys → driver unloads safely

🔹 DLL Mode (Silent Load via sys.ini)

1. Place SysUnlocked.dll in any folder
2. Create sys.ini next to it with your driver name:
   ```ini
   [Settings]
   Driver=MyDriver.sys
   ```
3. Place MyDriver.sys in the same folder
4. Load the DLL using any method (see below)
5. The driver loads silently — no GUI, no prompts
6. A message box appears: "Sys loaded successfully!"

---

🔧 How to Load the DLL

For a professional and easy way to load the DLL, check out:
👉 https://github.com/857seif/custom-dll-proxy

This project demonstrates a clean, reliable method for DLL proxying and loading. Use it as a reference for integrating SysUnlocked.dll into any application.

---

🔧 Build Instructions

```bash
1. Place your .sys file as resources/Loader.sys
2. Run build.bat
3. Choose: [1] EXE or [2] DLL
4. Get your file from bin/release/
```

---

🧪 Supported Systems

<p align="center">
  <img src="https://img.shields.io/badge/Windows%2010%2021H2+-FULLY%20SUPPORTED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Windows%2011%2022H2+-FULLY%20SUPPORTED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Windows%20Server%202019+-SUPPORTED-00FF00?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Windows%2010%20Older-MAY%20WORK-FFA500?style=for-the-badge&logo=windows&logoColor=white"/>
  <img src="https://img.shields.io/badge/Windows%2011%20Insider-MAY%20BREAK-FF0000?style=for-the-badge&logo=windows&logoColor=white"/>
</p>

---

🚀 Why "SysUnlocked"?

Because your .sys files should be unlocked — free from Microsoft's signature tyranny.
Because the kernel belongs to you, not to a certificate authority.
Because rules are for those who follow them. We write our own.

---

📜 License

MIT License — Free to use, modify, and share.
Use only on systems you own or have permission to test.

---

⚠️ Disclaimer

This tool is for educational and research purposes only.
The author is not responsible for any damage caused by misuse.
Drivers run at kernel level — they can crash your system or cause data loss.
Always test in a VM first.

---

🌟 Star This Repo

If you found this useful, give it a ⭐ and share it with your fellow kernel hackers!

---

💀 Final Word

"Rules are for those who follow them. We write our own."

---

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=00FF00&height=150&section=footer" alt="Footer" />
</p>

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=18&duration=3000&pause=1000&color=00FF00&center=true&vCenter=true&width=400&lines=BUILT+WITH+%E2%9D%A4%EF%B8%8F+AND+ASSEMBLY;KERNEL+IS+YOURS;UNLOCK+THE+SYSTEM" alt="Footer Text" />
</p>

<p align="center">
  <img src="https://komarev.com/ghpvc/?username=SysUnlocked&label=Views&color=00FF00&style=flat-square" alt="Views" />
</p>
