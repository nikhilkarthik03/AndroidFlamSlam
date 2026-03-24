// Top-level build file where you can add configuration options common to all sub-projects/modules.
plugins {
    alias(libs.plugins.android.application) apply false
}

subprojects {

    if (name == "app") {

        val compileShaders by tasks.registering(Exec::class) {
            workingDir = rootProject.projectDir

            commandLine(
                "/bin/bash",
                "${rootProject.projectDir}/compile_shaders.sh",
                "android",
                "web"
            )
        }

        tasks.matching { it.name == "preBuild" }.configureEach {
            dependsOn(compileShaders)
        }
    }
}