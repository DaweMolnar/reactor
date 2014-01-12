# coverage.mk

.PHONY: coverage
coverage: out/testUnits.cov.d

out/testUnits.cov.d: out/testUnits.cov
	$(if $Q,@echo "  GEN   $@")
	$Qgenhtml \
		$< \
		--output-directory $@ \
		--quiet

out/testUnits.cov: run_testUnits
	$(if $Q,@echo "  LCOV  $@")
	$Qlcov \
		--output-file $@ \
		--no-external \
		--capture \
		--directory out \
		--base-directory $(abspath .) \
		--quiet
