#find . -type f -name '*.h' -exec du -cb {} + | grep total$
#find . -type f -name '*.cpp' -exec du -cb {} + | grep total$
find . -type f \( -name '*.cpp' -o -name '*.h' \) \
    -exec du -cb {} + | grep total$
