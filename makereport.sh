files=(array color common fragment hashmap io lib3d main mapping matrix options raster raytrace render scene_core scenes shape util vertex)
{
    for i in "${files[@]}"
    do
        if [ -e src/$i.h ]; then
            echo "### $i.h"
            echo -e "\n\`\`\`cpp\n"
            cat src/$i.h 2>/dev/null
            echo -e "\n\`\`\`\n\n"
        fi
        if [ -e src/$i.c ]; then
            echo "### $i.c"
            echo -e "\n\`\`\`cpp\n"
            cat src/$i.c 2>/dev/null
            echo -e "\n\`\`\`\n\n"
        fi
    done
} > report.md