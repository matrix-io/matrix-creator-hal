{{#success build.status}}
  {{build.author}} just built `{{repo.name}}:{{build.branch}}` from <%%DRONE_COMMIT_LINK%%|#{{truncate build.commit 8}}>
  :new: {{build.message}}
  :debian: `libmatrixio-creator-hal_%%DISTRIBUTION%%-%%CODENAME%%-%%PKG_VER%%-%%COMPONENT%%_armhf.deb` 
  :debian: `libmatrixio-creator-hal-dev_%%DISTRIBUTION%%-%%CODENAME%%-%%PKG_VER%%-%%COMPONENT%%_armhf.deb` 
  Were published to `apt.matrix.one/%%DISTRIBUTION%% %%CODENAME%% %%COMPONENT%%`
{{else}}
  {{build.author}} just broke the build of `{{repo.name}}:{{build.branch}}` with <%%DRONE_COMMIT_LINK%%|#{{truncate build.commit 8}}>
  :new: :zombie: {{build.message}}
  :debian: `libmatrixio-creator-hal_%%DISTRIBUTION%%-%%CODENAME%%-%%PKG_VER%%-%%COMPONENT%%_armhf.deb` 
  :debian: `libmatrixio-creator-hal-dev_%%DISTRIBUTION%%-%%CODENAME%%-%%PKG_VER%%-%%COMPONENT%%_armhf.deb` 
  Failed to build
{{/success}}
:stopwatch: {{ since build.started}}
:gear: {{build.link}}
