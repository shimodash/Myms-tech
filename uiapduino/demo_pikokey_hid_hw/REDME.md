# demo_pikokey_hid_hw

CH32V003 を USB HID キーボードとして動作させ、ボタンを押すと指定したURLを自動入力するデモです。

ボタンを押すと `https://claude.ai/` + Enter が入力されます。

## ハードウェア

| 部品 | 説明 |
|---|---|
| uiapduino (CH32V003搭載) | メインマイコンボード |
| タクトスイッチ | PC1 (3番ピン) - GND 間に接続 |

### ピン配置

```
uiapduino 3番ピン (PC1) ── タクトスイッチ ── GND
```

- PC1 は内部プルアップで使用します。外部プルアップ抵抗は不要です。
- USB D+ は PD3、USB D- は PD4 に接続してください。

## ソフトウェア構成

| ファイル | 説明 |
|---|---|
| `pikokey.c` | メインソース。入力文字列・ボタンピンはここで変更 |
| `funconfig.h` | ch32fun の設定 |
| `usb_config.h` | USB HID キーボードのディスクリプタ定義 |
| `Makefile` | ビルド設定 |

## ビルド環境

- Ubuntu 24.04 (WSL2)
- [xpack RISC-V GCC](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack) v15.2.0
- [ch32fun](https://github.com/cnlohr/ch32fun)
- [rv003usb](https://github.com/cnlohr/rv003usb)

### ディレクトリ構成

```
~/
├── ch32fun/        # ch32fun リポジトリ
└── rv003usb/
    └── demo_pikokey_hid_hw/   # 本リポジトリ
```

### ビルド手順

```bash
# 依存リポジトリのクローン
cd ~
git clone --recursive https://github.com/cnlohr/ch32fun
git clone https://github.com/cnlohr/rv003usb

# xpack GCC のインストール
curl -L https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v15.2.0-1/xpack-riscv-none-elf-gcc-15.2.0-1-linux-x64.tar.gz | tar -xz -C ~
echo 'export PATH="$HOME/xpack-riscv-none-elf-gcc-15.2.0-1/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc

# 本リポジトリのファイルを配置
cp <このリポジトリのファイル> ~/rv003usb/demo_pikokey_hid_hw/

# ビルド & 書き込み
cd ~/rv003usb/demo_pikokey_hid_hw
make flash
```

### WSL2 での USB 書き込み

[usbipd-win](https://github.com/dorssel/usbipd-win) を使って CH32V003 を WSL2 に接続します。

```powershell
# Windows PowerShell (管理者)
usbipd list
usbipd attach --wsl --busid <BUSID>
```

## 入力文字列の変更

`pikokey.c` の `string_keys[]` を編集してください。
各エントリは `{modifier, keycode}` の形式です。

```c
static const KeyEntry string_keys[] = {
    {0x00, 0x0b}, // Usage ID 0x0b  'h'
    ...
};
```

JIS配列の主要なUsage IDは `pikokey.c` 冒頭のコメントを参照してください。

## ライセンス

本プロジェクトは [ch32fun](https://github.com/cnlohr/ch32fun) および
[rv003usb](https://github.com/cnlohr/rv003usb) を使用しています。
