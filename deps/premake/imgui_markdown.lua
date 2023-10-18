imgui_markdown = {
    source = path.join(dependencies.basePath, "imgui_markdown"),
}

function imgui_markdown.import()
    imgui_markdown.includes()
end

function imgui_markdown.includes()
    includedirs {
        imgui_markdown.source
    }
end

function imgui_markdown.project()
end

table.insert(dependencies, imgui_markdown)