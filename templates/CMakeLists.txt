#=======================================
# Template Files
#=======================================
set (dtd_files
  glabels-3.0.dtd
  glabels-4.0.dtd
)

set (template_files
  ascom-iso-templates.xml
  avery-iso-templates.xml
  avery-other-templates.xml
  avery-us-templates.xml
  begalabel-templates.xml
  brother-other-templates.xml
  cablelabel-templates.xml
  canon-other-templates.xml
  databecker-iso-templates.xml
  dataline-iso-templates.xml
  decadry-iso-templates.xml
  desmat-templates.xml
  dymo-other-templates.xml
  endisch-templates.xml
  felga-templates.xml
  geha-iso-templates.xml
  hama-iso-templates.xml
  herma-iso-templates.xml
  hisago-templates.xml
  igepa-templates.xml
  jac-iso-templates.xml
  maco-us-templates.xml
  mayspies-templates.xml
  meritline-us-templates.xml
  microapp-templates.xml
  misc-iso-templates.xml
  misc-other-templates.xml
  misc-us-templates.xml
  online-templates.xml
  pearl-iso-templates.xml
  phomemo-templates.xml
  rayfilm-templates.xml
  sheetlabels-us-templates.xml
  uline-us-templates.xml
  worldlabel-us-templates.xml
  zweckform-iso-templates.xml
)

set (other_db_files
  categories.xml
  paper-sizes.xml
  vendors.xml
)

# Export variables to parent scope
set (template_files ${template_files} PARENT_SCOPE)
set (other_db_files ${other_db_files} PARENT_SCOPE)

#=======================================
# Install
#=======================================
install (FILES ${dtd_files}      DESTINATION share/glabels-qt/templates)
install (FILES ${template_files} DESTINATION share/glabels-qt/templates)
install (FILES ${other_db_files} DESTINATION share/glabels-qt/templates)
