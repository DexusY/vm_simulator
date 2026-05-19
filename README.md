# vm_simulator

A bytecode virtual machine for a small 8-bit, hybrid stack/register ISA. Written in C11, built with CMake, tested with CTest.

The point isn't to emulate any specific hardware. It's a sandbox for working through instruction decoding, status-flag arithmetic, and execution semantics in a surface area small enough to keep entirely in your head — and disciplined enough to fail loudly when something goes wrong.

## Architecture

- **Word size:** 8-bit. All arithmetic results are truncated to a byte.
- **Memory:** 256 bytes, flat, addressable `0x00`–`0xFF`. Code, data, and stack share one region.
- **Registers:** `R0`–`R3`, plus `PC`, `SP`, and a status byte.
- **Status flags:** `ZF` (zero), `CF` (unsigned carry), `SF` (sign), `OF` (signed overflow). x86-style semantics.
- **Stack:** grows downward from `0xFF`; `SP = 0` is the empty state. A push that would collide with `PC` is rejected as overflow.
- **Execution:** `cpu_step` fetches one opcode from `memory[PC]`, advances `PC`, dispatches by opcode. Each handler is responsible for its own operand fetches and flag updates.

## Project layout

```
vm_simulator/
├── main.c                        entry point and a sample program
├── cpu.{c,h}                     fetch / decode / dispatch loop
├── vm_config.h                   opcodes, CPU struct, memory and register sizes, flag bits
├── flag.{c,h}                    ZF / CF / SF / OF computation, shared by ALU and CMP
├── program_loader.{c,h}          loads a byte array into VM memory
├── instructions_alu.{c,h}        ADD SUB MUL DIV MOD AND OR XOR NOT SHL SHR
├── instructions_memory.{c,h}     SET LOAD STORE MOV
├── instructions_stack.{c,h}      PSH POP DUP SWAP NOP
├── instructions_control.{c,h}    HLT CMP JMP JZ JNZ JG JL CALL RET
└── tests/                        per-module test binaries, wired through CTest
```

Each instruction category lives in its own translation unit. Adding a new instruction means: extend the opcode enum in `vm_config.h`, write the handler in the right `instructions_*.c`, add a `case` to `cpu_step`.

## Building

Requires CMake `>= 3.16` and a C11 compiler.

```sh
cmake -S . -B build
cmake --build build
```

The tree builds clean under `-Wall -Wextra -Wpedantic -Wshadow -Wconversion`. Debug is the default; pass `-DCMAKE_BUILD_TYPE=Release` for `-O2`.

## Running

```sh
./build/vm_simulator
```

The sample program in `main.c` pushes two values, adds them, pops the result, and halts. Every handler emits a trace line so execution can be followed by eye:

```
[Stack] Pushed value 5 at address 0xFF
[Stack] Pushed value 6 at address 0xFE
[ALU] ADD 6 + 5 = 11
[Flags] ZF=0 CF=0 SF=0 OF=0
[Stack] Popped value 11
[CPU] HLT
```

## Instruction set

| Opcode             | Operands     | Effect                                                          |
|--------------------|--------------|-----------------------------------------------------------------|
| `HLT`              | —            | Stop execution.                                                 |
| `PSH`              | `imm8`       | Push `imm8` onto the stack.                                     |
| `POP`              | —            | Discard the top of the stack.                                   |
| `DUP`              | —            | Duplicate the top of the stack.                                 |
| `SWAP`             | —            | Swap the top two elements.                                      |
| `NOP`              | —            | Do nothing.                                                     |
| `ADD` / `SUB` / `MUL` / `DIV` / `MOD` | — | Pop two operands, push the result. Updates flags.       |
| `AND` / `OR` / `XOR`                  | — | Bitwise op on the top two. Updates flags.               |
| `NOT`              | —            | Bitwise negation of the top. Updates flags.                     |
| `SHL` / `SHR`      | —            | Shift the top by `n` bits taken from the next slot, `0 <= n < 8`. |
| `SET`              | `reg`, `imm8`| `Rreg = imm8`.                                                  |
| `LOAD`             | `reg`, `addr`| `Rreg = memory[addr]`.                                          |
| `STORE`            | `reg`, `addr`| `memory[addr] = Rreg`.                                          |
| `MOV`              | `dst`, `src` | `Rdst = Rsrc`.                                                  |
| `CMP`              | `reg`, `imm8`| Updates flags as if `Rreg - imm8` were computed.                |
| `JMP`              | `addr`       | `PC = addr`.                                                    |
| `JZ` / `JNZ`       | `addr`       | Branch on `ZF`.                                                 |
| `JG` / `JL`        | `addr`       | Signed greater / less. Uses `SF`, `OF`, `ZF`.                   |
| `CALL`             | `addr`       | Push current `PC`, then `PC = addr`.                            |
| `RET`              | —            | Pop into `PC`.                                                  |

Binary ALU operations consume the top two stack slots in the order `top OP next`. This matters for `SUB`, `DIV`, `MOD`, and the shifts.

## Tests

```sh
ctest --test-dir build --output-on-failure
```

Tests live in `tests/`, one binary per module. Coverage is by behaviour, not by line:

- `test_alu` — arithmetic, logic, shifts, and each flag-affecting edge case (zero result, unsigned wrap, signed overflow, carry out on shift).
- `test_memory` — `SET` / `LOAD` / `STORE` / `MOV`, including invalid-register guards.
- `test_stack` — push / pop / dup / swap, plus underflow and the "fewer than two elements" branch in `SWAP`.
- `test_control` — every conditional jump in both directions, plus a `CALL` / `RET` round-trip that verifies the return address is restored to `PC`.
- `test_flag` — `update_flags` in isolation, one assertion per flag.
- `test_program_loader` — byte-copy semantics and the untouched-region invariant.
- `test_cpu` — end-to-end programs driven through `cpu_step`, including a taken vs. not-taken conditional jump pair.

The harness is a single header (`tests/test_framework.h`) with `ASSERT_EQ`, `ASSERT_TRUE`, `ASSERT_FLAG_SET` / `_CLEAR`, and a `RUN_TEST` runner — no external framework, no extra dependency to vendor in. `silence_stdout()` redirects the simulator's trace output to `/dev/null` so the test report stays readable.

## Design notes

- **Flag computation is centralised.** ALU and `CMP` both go through `update_flags(cpu, result16, a, b)`. `CMP` passes `~b` instead of `b` so the same signed-overflow formula written for addition gives the correct answer for subtraction: subtracting `b` has the same overflow behaviour as adding its one's complement (the missing `+1` only affects the LSB, not the sign).
- **Carry comes for free.** Arithmetic ops compute into a `uint16_t` first; anything above `0xFF` sets `CF` without a separate check, and the cast back to `uint8_t` produces the correct wrapped result.
- **Error paths are deliberately lossy.** `DIV` / `MOD` by zero and out-of-range shifts consume their stack operands but do not push a result. The VM logs the error and continues — there is no exception mechanism, and tests pin this exact behaviour so it can't drift silently.
- **Stack overflow is checked positionally.** `PSH` rejects a push that would land on `PC`, rather than tracking a separate stack boundary, since code and data share one address space.
- **Modular dispatch.** `cpu_step` is a flat `switch` over the opcode enum; each `case` is one or two lines. Adding an instruction touches three places (enum, handler file, switch), and nothing else.
