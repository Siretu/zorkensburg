find . -regex ".*\.\(cpp\|h\)" -exec cat {} + | wc -l
