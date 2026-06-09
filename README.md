# std-crypto-bench

## 개요

대칭키 블록 암호 알고리즘을 C로 직접 구현하고, IPB(Instructions per Byte) 지표로 성능을 비교하는 벤치마크.

- 측정 도구: Valgrind callgrind
- 성능 지표: IPB = 명령어 수(Ir) / 처리 바이트 수
  - 동일한 컴파일러·플래그 조건에서 하드웨어에 무관하게 일정한 값 산출



## 대상 알고리즘

선정 기준:

- 국제 또는 국가 표준으로 공식 채택
- 블록 암호 방식
- 서로 다른 설계 구조 포함 (SPN / Feistel)
- 공개된 사양 문서 존재


| Algorithm | Structure | Key (bits) | Block (bits) | Rounds | Standard       |
|--|--|--|--|--|-|
| AES-128   | SPN       | 128        | 128          | 10     | NIST FIPS 197  |
| AES-192   | SPN       | 192        | 128          | 12     | NIST FIPS 197  |
| AES-256   | SPN       | 256        | 128          | 14     | NIST FIPS 197  |
| ARIA-128  | SPN       | 128        | 128          | 12     | KS X 1213-1    |
| SEED      | Feistel   | 128        | 128          | 16     | KS X 1213      |
| DES       | Feistel   | 56         | 64           | 16     | FIPS 46-3      |
| 3DES-2KEY | Feistel   | 112        | 64           | 48     | NIST SP 800-67 |
| 3DES-3KEY | Feistel   | 168        | 64           | 48     | NIST SP 800-67 |

> DES, 3DES는 폐기된 표준. 알고리즘 구조 비교 목적으로만 포함.



## AES 변종 시나리오

AES 변종은 연구 문제 2를 위해 별도 실행한다. 성능 지표와 측정 조건은 표준 알고리즘 벤치마크와 동일하게 IPB를 사용한다.

### Scenario A: 라운드 수 변경

키 길이를 AES-128과 동일한 128 bit로 고정하고 라운드 수만 변경한다. 기준 알고리즘은 `AES-128`이다.

| Algorithm   | Key (bits) | Block (bits) | Rounds | Note             |
|--|--:|--:|--:|--|
| AES-128-R8  | 128        | 128          | 8      | Reduced rounds   |
| AES-128     | 128        | 128          | 10     | Baseline         |
| AES-128-R12 | 128        | 128          | 12     | Increased rounds |

`AES-128-R8`, `AES-128-R12`는 표준 AES가 아니므로 공식 ciphertext 테스트 벡터는 없다. 구현 검증은 암호화 후 복호화 round-trip으로 수행한다.

### Scenario B: 키 길이와 표준 라운드 수 변경

NIST FIPS 197의 표준 AES 키 길이와 라운드 수를 사용한다. 기준 알고리즘은 `AES-128`이다.

| Algorithm | Key (bits) | Block (bits) | Rounds | Note     |
|--|--:|--:|--:|--|
| AES-128   | 128        | 128          | 10     | Baseline |
| AES-192   | 192        | 128          | 12     | Standard |
| AES-256   | 256        | 128          | 14     | Standard |



## 측정 방식

```
Metric  : IPB = Ir / plaintext bytes
Tool    : Valgrind callgrind, flags: --cache-sim=no --branch-sim=no
Compiler: GCC, flags: -O2 -mno-aes -march=x86-64 -std=c99
                      -fno-inline -fno-ipa-cp -fno-ipa-sra
                      -fno-optimize-sibling-calls
                      -fno-tree-loop-vectorize -fno-tree-slp-vectorize
                      -fno-ivopts -fno-unroll-loops
Link    : -static
Mode    : ECB
Input   : 1 MB, 10 MB
Warmup  : 3 iterations
Repeat  : 10 iterations
OS      : Linux
```

- `-mno-aes`: x86 AES-NI 비활성화. AES와 나머지 알고리즘 간 하드웨어 가속 차이를 배제하고 소프트웨어 구현만 비교
- `-fno-tree-loop-vectorize` `-fno-tree-slp-vectorize`: SIMD 자동 벡터화 비활성화. 알고리즘 간 명령어 구성이 달라져 Ir 비교를 왜곡
- `-fno-ivopts`: 귀납 변수 최적화 비활성화. 루프 카운터 강도 감소 변환으로 Ir이 달라짐
- `-fno-unroll-loops`: 루프 언롤링 비활성화. GCC 버전에 따라 소형 루프(반복 횟수 <= 16) 언롤 여부가 달라져 Ir이 변동되는 문제를 억제
- 벤치마크 포함 기준: NIST CAVP 또는 KISA 공식 테스트 벡터 통과 구현에 한함



## 디렉토리 구조

```
StdCryptoBench/
├── include/
│   ├── aes.h
│   ├── aes_variant.h
│   ├── aria.h
│   ├── seed.h
│   ├── des.h
│   └── 3des.h
├── src/
│   ├── aes_internal.h
│   ├── aes.c
│   ├── aes_variant.c
│   ├── aria.c
│   ├── seed.c
│   ├── des.c
│   └── 3des.c
├── bench/
│   ├── harness.h
│   ├── harness.c
│   └── main.c
├── test/
│   ├── test_runner.c
│   ├── test_aes.c
│   ├── test_aes_variant.c
│   ├── test_aria.c
│   ├── test_seed.c
│   ├── test_des.c
│   └── vectors/
│       ├── aes_ecb.txt
│       ├── aria_ecb.txt
│       ├── seed_ecb.txt
│       └── des_ecb.txt
├── scripts/
│   ├── bench.sh
│   ├── bench_aes_rounds.sh
│   ├── bench_aes_keys.sh
│   └── parse_ipb.py
└── Makefile
```

| 디렉토리          | 역할 |
|--|--|
| `include/`        | 알고리즘 공개 API 헤더. `bench/`, `test/`는 이곳만 참조. |
| `src/`            | 알고리즘 구현. `aes_internal.h`는 `aes.c`, `aes_variant.c` 전용 내부 헤더. |
| `bench/`          | callgrind 기반 벤치마크 하네스. 측정 구간 한정, 워밍업, IPB 계산 담당. |
| `test/`           | NIST CAVP 및 KISA 테스트 벡터 기반 구현 정확성 검증. |
| `test/vectors/`   | 테스트 벡터 데이터 파일. |
| `scripts/`        | 표준 알고리즘 및 AES 변종 벤치마크 실행 스크립트. `parse_ipb.py`: callgrind 출력 파싱 및 IPB 계산. |



## 요구 환경

- OS: Linux
- GCC >= 11
- Valgrind >= 3.20
- Python >= 3.8



## 빌드

```bash
make
```

- 벤치마크 워커 바이너리: `bin/bench` (정적 링크)

테스트:

```bash
make test
```



## 실행

```bash
./scripts/bench.sh
./scripts/bench_aes_rounds.sh
./scripts/bench_aes_keys.sh
```

- 표준 알고리즘 벤치마크 진입점은 `scripts/bench.sh`
- AES 라운드 수 변종은 `./scripts/bench_aes_rounds.sh`로 별도 실행
- AES 키 길이 변종은 `./scripts/bench_aes_keys.sh`로 별도 실행
- `bin/bench`는 최소한의 인자 검증만 수행함 `scripts/bench.sh`가 유효한 인자를 전달한다고 가정
- callgrind 출력: 프로젝트 루트에 `callgrind_${algo}_${size}MB_${dir}.out`로 저장
- IPB 계산 결과: 표준 출력



## 라이선스

MIT License. [LICENSE](./LICENSE) 참조.
