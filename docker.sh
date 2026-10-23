docker run -it \
  --name s3k-colab-CFI \
  -v "$(pwd -P):/workspace" \
  -w /workspace \
  hakarlsson/riscv-picolibc
