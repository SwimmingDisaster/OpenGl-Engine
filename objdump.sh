mkdir -p $1/code
mkdir -p $1/code/assets
mkdir -p $1/code/utils
mkdir -p $1/code/core/other
mkdir -p $1/code/core/ui
mkdir -p $1/code/ecs/other

mkdir -p $1/vendor/GLAD
mkdir -p $1/vendor/ImGUI
mkdir -p $1/vendor/imguizmo
mkdir -p $1/vendor/yaml-cpp/contrib



objdump -drwC -Mintel obj/code/assets/deleteAfterTouched.o $1/code/assets/deleteAfterTouched.o
objdump -drwC -Mintel obj/code/assets/camera.o > $1/code/assets/camera.o
objdump -drwC -Mintel obj/code/assets/cameraFPSController.o > $1/code/assets/cameraFPSController.o
objdump -drwC -Mintel obj/code/assets/mesh.o > $1/code/assets/mesh.o
objdump -drwC -Mintel obj/code/assets/animator.o > $1/code/assets/animator.o
objdump -drwC -Mintel obj/code/assets/material.o > $1/code/assets/material.o
objdump -drwC -Mintel obj/code/assets/colliders.o > $1/code/assets/colliders.o
objdump -drwC -Mintel obj/code/assets/physicsFPScontroller.o > $1/code/assets/physicsFPScontroller.o
objdump -drwC -Mintel obj/code/assets/testComponent.o > $1/code/assets/testComponent.o
objdump -drwC -Mintel obj/code/assets/model.o > $1/code/assets/model.o
objdump -drwC -Mintel obj/code/assets/transform.o > $1/code/assets/transform.o
objdump -drwC -Mintel obj/code/assets/rigidbody.o > $1/code/assets/rigidbody.o
objdump -drwC -Mintel obj/code/assets/modelRenderer.o > $1/code/assets/modelRenderer.o
objdump -drwC -Mintel obj/code/assets/colliderBase.o > $1/code/assets/colliderBase.o
objdump -drwC -Mintel obj/code/utils/fileUtils.o > $1/code/utils/fileUtils.o
objdump -drwC -Mintel obj/code/core/input.o > $1/code/core/input.o
objdump -drwC -Mintel obj/code/core/physics.o > $1/code/core/physics.o
objdump -drwC -Mintel obj/code/core/batchRenderer.o > $1/code/core/batchRenderer.o
objdump -drwC -Mintel obj/code/core/renderer.o > $1/code/core/renderer.o
objdump -drwC -Mintel obj/code/core/texture.o > $1/code/core/texture.o
objdump -drwC -Mintel obj/code/core/other/log.o > $1/code/core/other/log.o
objdump -drwC -Mintel obj/code/core/other/editorCamera.o > $1/code/core/other/editorCamera.o
objdump -drwC -Mintel obj/code/core/other/random.o > $1/code/core/other/random.o
objdump -drwC -Mintel obj/code/core/other/helpers.o > $1/code/core/other/helpers.o
objdump -drwC -Mintel obj/code/core/application.o > $1/code/core/application.o
objdump -drwC -Mintel obj/code/core/engineInfo.o > $1/code/core/engineInfo.o
objdump -drwC -Mintel obj/code/core/layer.o > $1/code/core/layer.o
objdump -drwC -Mintel obj/code/core/main.o > $1/code/core/main.o
objdump -drwC -Mintel obj/code/core/imGuiManager.o > $1/code/core/imGuiManager.o
objdump -drwC -Mintel obj/code/core/shader.o > $1/code/core/shader.o
objdump -drwC -Mintel obj/code/core/framebuffer.o > $1/code/core/framebuffer.o
objdump -drwC -Mintel obj/code/core/math.o > $1/code/core/math.o
objdump -drwC -Mintel obj/code/core/physxSimulationFilter.o > $1/code/core/physxSimulationFilter.o
objdump -drwC -Mintel obj/code/core/ui/text.o > $1/code/core/ui/text.o
objdump -drwC -Mintel obj/code/core/modelImporter.o > $1/code/core/modelImporter.o
objdump -drwC -Mintel obj/code/core/tag.o > $1/code/core/tag.o
objdump -drwC -Mintel obj/code/ecs/component.o > $1/code/ecs/component.o
objdump -drwC -Mintel obj/code/ecs/other/componentFactory.o > $1/code/ecs/other/componentFactory.o
objdump -drwC -Mintel obj/code/ecs/other/componentCreatorImpl.o > $1/code/ecs/other/componentCreatorImpl.o
objdump -drwC -Mintel obj/code/ecs/other/componentCreator.o > $1/code/ecs/other/componentCreator.o
objdump -drwC -Mintel obj/code/ecs/scene.o > $1/code/ecs/scene.o
objdump -drwC -Mintel obj/code/ecs/entity.o > $1/code/ecs/entity.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui.o > $1/vendor/ImGUI/imgui.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_demo.o > $1/vendor/ImGUI/imgui_demo.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_tables.o > $1/vendor/ImGUI/imgui_tables.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_widgets.o > $1/vendor/ImGUI/imgui_widgets.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_stdlib.o > $1/vendor/ImGUI/imgui_stdlib.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_impl_opengl3.o > $1/vendor/ImGUI/imgui_impl_opengl3.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_draw.o > $1/vendor/ImGUI/imgui_draw.o
objdump -drwC -Mintel obj/vendor/ImGUI/imgui_impl_glfw.o > $1/vendor/ImGUI/imgui_impl_glfw.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/nodebuilder.o > $1/vendor/yaml-cpp/nodebuilder.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/emitter.o > $1/vendor/yaml-cpp/emitter.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/null.o > $1/vendor/yaml-cpp/null.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/emit.o > $1/vendor/yaml-cpp/emit.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/node_data.o > $1/vendor/yaml-cpp/node_data.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/memory.o > $1/vendor/yaml-cpp/memory.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/scanner.o > $1/vendor/yaml-cpp/scanner.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/emitterstate.o > $1/vendor/yaml-cpp/emitterstate.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/exceptions.o > $1/vendor/yaml-cpp/exceptions.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/convert.o > $1/vendor/yaml-cpp/convert.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/nodeevents.o > $1/vendor/yaml-cpp/nodeevents.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/ostream_wrapper.o > $1/vendor/yaml-cpp/ostream_wrapper.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/singledocparser.o > $1/vendor/yaml-cpp/singledocparser.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/scanscalar.o > $1/vendor/yaml-cpp/scanscalar.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/simplekey.o > $1/vendor/yaml-cpp/simplekey.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/scantoken.o > $1/vendor/yaml-cpp/scantoken.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/depthguard.o > $1/vendor/yaml-cpp/depthguard.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/binary.o > $1/vendor/yaml-cpp/binary.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/directives.o > $1/vendor/yaml-cpp/directives.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/parser.o > $1/vendor/yaml-cpp/parser.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/parse.o > $1/vendor/yaml-cpp/parse.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/emitfromevents.o > $1/vendor/yaml-cpp/emitfromevents.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/scantag.o > $1/vendor/yaml-cpp/scantag.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/regex_yaml.o > $1/vendor/yaml-cpp/regex_yaml.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/contrib/graphbuilderadapter.o > $1/vendor/yaml-cpp/contrib/graphbuilderadapter.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/contrib/graphbuilder.o > $1/vendor/yaml-cpp/contrib/graphbuilder.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/emitterutils.o > $1/vendor/yaml-cpp/emitterutils.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/tag.o > $1/vendor/yaml-cpp/tag.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/exp.o > $1/vendor/yaml-cpp/exp.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/node.o > $1/vendor/yaml-cpp/node.o
objdump -drwC -Mintel obj/vendor/yaml-cpp/stream.o > $1/vendor/yaml-cpp/stream.o
objdump -drwC -Mintel obj/vendor/GLAD/glad.o > $1/vendor/GLAD/glad.o
objdump -drwC -Mintel obj/vendor/imguizmo/ImGuizmo.o > $1/vendor/imguizmo/ImGuizmo.o
objdump -drwC -Mintel obj/vendor/imguizmo/ImGradient.o > $1/vendor/imguizmo/ImGradient.o
objdump -drwC -Mintel obj/vendor/imguizmo/ImCurveEdit.o > $1/vendor/imguizmo/ImCurveEdit.o
objdump -drwC -Mintel obj/vendor/imguizmo/ImSequencer.o > $1/vendor/imguizmo/ImSequencer.o
objdump -drwC -Mintel obj/mypch.o > $1/mypch.o
