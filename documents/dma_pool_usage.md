# DMA Pool 使用說明

本文件說明 DMA pool 的配置與使用方式，參考 `middleware/wise_shell/commands/cmd_crypto.c`、`wise_core_v2/api/wise_core.h` 中的 DMA 巨集，以及 `app/wise_core_trunk/ldscripts/default_9006.ld` 與 `wise_core_v2/platform/subg_soc_9006/arm_core/cmsis/esmt/Source/startup_er81xx.c` 的配置與初始化流程。

## 1. 目的與概念

DMA 需要資料位於特定可 DMA 的 RAM 區域。專案透過 linker script 定義 `.dma_data` 與 `.dma_bss` 區段，並提供巨集把 Buffer 放進 DMA pool。

- `.dma_data`: 有初始值的 DMA buffer，開機時從 Flash 複製到 RAM。
- `.dma_bss`: 無初始值的 DMA buffer，開機時清為 0。

## 1.1 設計理由與合理性

這樣的設計邏輯上是合理的，主要目標是同時滿足 DMA 可用性、初始化成本、記憶體佔用與開機可預期性：

- **區分 `.dma_data` 與 `.dma_bss`**：需要初始化值（例如金鑰、測試向量）的 DMA buffer 必須從 Flash 複製到 RAM，放在 `.dma_data` 可自動完成；輸出或暫存類型的 buffer 放在 `.dma_bss`，開機自動清零，避免多餘的 Flash 佔用與複製時間。
- **可預期的啟動流程**：`dma_data_init()` 與 `dma_bss_init()` 與既有 `.data` / `.bss` 初始化一致，讓 DMA 相關資料在任何使用點都已準備好，減少 runtime 初始化負擔。
- **區段與符號集中管理**：DMA pool 的範圍由 linker 統一管理並在 link-time 檢查（容量、對齊、重疊），比起手動分散配置更安全可控。
- **巨集化宣告**：讓 DMA buffer 宣告更一致，減少開發者誤放位置或忘記對齊導致 DMA 失敗的機率。

## 2. 相關巨集（`wise_core_v2/api/wise_core.h`）

當編譯選項定義 `DMA_POOL` 時可使用以下巨集：

- `DMA_ATTR`: 全域/檔案層級，放入 `.dma_pool`，零初始化。
- `DMA_DATA_ATTR`: 全域/檔案層級，放入 `.dma_data`，支援初始化值。
- `STATIC_DMA`: 函式內 static buffer，放入 `.dma_pool`。
- `STATIC_DMA_DATA`: 函式內 static buffer，放入 `.dma_data`。

範例：

```c
uint8_t rx_buf[256] DMA_ATTR;
const uint8_t aes_key[16] DMA_DATA_ATTR = {0x2B, 0x7E, ...};

STATIC_DMA uint8_t out[64];
STATIC_DMA_DATA uint8_t in[64] = {0x6B, 0xC1, ...};
```

## 3. Linker Script 配置（`app/wise_core_trunk/ldscripts/default_9006.ld`）

Linker script 定義 DMA pool 的區段與符號：

- `.dma_data`：放 `*(.dma_data*)`
- `.dma_bss`：放 `*(.dma_pool*)` 與 `*(.dma_buffer*)`
- 產生符號：`__dma_data_start__`、`__dma_data_end__`、`__dma_bss_start__`、`__dma_bss_end__`
- 匯總符號：`__dma_pool_start__`、`__dma_pool_end__`、`__dma_size__`

同時包含保護性檢查：

- DMA 區域必須在 48KB hardware limit 內
- DMA pool 需 4-byte 對齊
- DMA 區域不可與 heap 重疊

## 4. Startup 初始化（`startup_er81xx.c`）

當定義 `DMA_POOL` 時，`Reset_Handler` 會呼叫：

- `dma_data_init()`：把 `.dma_data` 從 Flash 複製到 RAM
- `dma_bss_init()`：把 `.dma_bss` 清為 0

這與一般 `.data` / `.bss` 初始化流程一致，確保 DMA buffer 在使用前已正確初始化。

## 5. 實際用法（參考 `cmd_crypto.c`）

`cmd_crypto.c` 中大量使用 `STATIC_DMA_DATA` 與 `STATIC_DMA` 來確保 AES/SHA 所需的 buffer 位於 DMA pool，例如：

```c
STATIC_DMA_DATA uint8_t aes_key[16] = { ... };
STATIC_DMA_DATA uint8_t plaintext[64] = { ... };
STATIC_DMA uint8_t output[64];
```

建議規則：

- **輸入、常數、金鑰**：使用 `DMA_DATA_ATTR` 或 `STATIC_DMA_DATA`，保留初始化值。
- **輸出 buffer 或暫存**：使用 `DMA_ATTR` 或 `STATIC_DMA`，由 startup 清零。

### 5.1 使用範例

全域輸出 buffer（零初始化）：

```c
uint8_t dma_tx_buf[512] DMA_ATTR;
```

全域常數金鑰（保留初始化值）：

```c
const uint8_t aes_key[16] DMA_DATA_ATTR = {
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
```

函式內暫存 buffer（零初始化）：

```c
void do_crypto(void)
{
    STATIC_DMA uint8_t output[64];
    /* output 會在 startup 清零 */
}
```

函式內測試向量（保留初始化值）：

```c
void do_crypto(void)
{
    STATIC_DMA_DATA uint8_t plaintext[64] = { 0x6B, 0xC1, ... };
    /* plaintext 會從 Flash 複製到 RAM */
}
```

## 6. 使用步驟總結

1. 在編譯選項中定義 `DMA_POOL`。
2. 確認 linker script（如 `default_9006.ld`）包含 `.dma_data` / `.dma_bss` 區段與符號。
3. 確認 startup 檔案包含 `dma_data_init()` / `dma_bss_init()`。
4. 依需求使用 `DMA_ATTR` / `DMA_DATA_ATTR` / `STATIC_DMA` / `STATIC_DMA_DATA` 宣告 DMA buffer。

## 7. 注意事項

- DMA buffer 需 4-byte 對齊（linker script 有 assert）。
- DMA pool 受硬體大小限制，超出會在 link-time 失敗。
- 若未定義 `DMA_POOL`，這些巨集會退化為空定義（不進入 DMA pool）。
