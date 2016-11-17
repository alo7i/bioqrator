var passages;
var annotations;
var relations;

function clearAllHighlight() {
  $("tr.type").removeClass("selected");
  $("tr.relation").removeClass("selected");
  $("tr.annotation_tr").removeClass("selected");
  $(".relation").removeClass("highlight");
  $(".annotation").removeClass("highlight");
  $(".annotation_tr").removeClass("highlight");
  $(".annotation").removeClass("highlight0");
  $(".annotation").removeClass("highlight1");
  $(".annotation").removeClass("highlight2");
  $(".annotation").removeClass("highlight3");
  $(".annotation").removeClass("highlight4");
  $(".annotation_tr").removeClass("highlight0");
  $(".annotation_tr").removeClass("highlight1");
  $(".annotation_tr").removeClass("highlight2");
  $(".annotation_tr").removeClass("highlight3");
  $(".annotation_tr").removeClass("highlight4");
  $("span.ref").removeClass("highlight0");
  $("span.ref").removeClass("highlight1");
  $("span.ref").removeClass("highlight2");
  $("span.ref").removeClass("highlight3");
  $("span.ref").removeClass("highlight4");
}


function ajax_document_fetch(id, collection_id) {
  $.ajax({
    type: "GET",
    url: "/documents/fetch",
    data:  { id: id,
             collection_id: collection_id
           },
    dataType: 'html',
    complete: function(html) {
      $("div#pmid-"+ id).find("i.fetching").hide();
    },
    success: function(html) {
      $("div#pmid-"+ id).replaceWith(html);
    },
    error: function(html) {
      $("div#pmid-"+ id +" span").html(" : Sorry, fail to fetch the content of the article. Please <button type='button' onclick='ajax_document_fetch(" + id + ");'>try again.</button> ");
    }
  });
}

function tpos2hpos(str, offset, len, pos) {
  var i = 0, tpos = 0;
  var ch, mode = 0;
  var target = pos - offset;

  if (offset + len < pos)
    return -1;

  while(i < str.length) {
    ch = str.charAt(i);
    if (mode === 0) {
      if (ch == "<") {
        mode = 1;
      } else {
        if (tpos == target) {
          return i;
        }
        if (ch == "&") {
          mode = 4;
        }
        tpos++;
      }
    } else if (mode == 1) {
      if (ch == ">") {
        mode = 0;
      } else if (ch == '"') {
        mode = 2;
      } else if (ch == "'") {
        mode = 3;
      }
    } else if (mode == 2) {
      if (ch == '"') {
        mode = 1;
      }
    } else if (mode == 3) {
      if (ch == "'") {
        mode = 1;
      }
    } else if (mode == 4) {
      if (ch == ";") {
        mode = 0;
      }
    }
    i++;
  }
  return -1;
}

function manualEditEntity(type) {
  var ul = $("#annotationModal ul.additional-information");
  var previousId = "";
  ul.find("li").each(function(){
    var el = $(this);
    var input1 = $(this).find("input.span3");
    var input2 = $(this).find("input.span2");
    if (type && type !== "" && type == input1.val()) {
      previousId = input2.val();
    }
  });

  var newId = prompt("Enter " + type + " ID ", previousId);
  if (newId) {
    recommendEntity(type, newId, null, null, true);
  }
}

function addAdditionalInformation(target, type, value, desc, link, forceUpdate) {
  var ul = $("#"+target + "Modal ul.additional-information");
  ul.find("li").each(function(){
    var el = $(this);
    var input1 = $(this).find("input.span3");
    var input2 = $(this).find("input.span2");
    if (type && type !== "" && type == input1.val()) {
      if (forceUpdate === true || confirm("Replace current item '"+type+"' ?")) {
        el.remove();
      }
    }
  });

  var cls = "";

  if (type == "UniProtKB" || type == "EntrezGene") {
    cls = "readonly";
  }

  var html = "<li><input type='text' class='span3' name='info_types[]' placeholder='Key / Database (e.g. EntrezGene)' value='" + type + "' " + cls + "> " +
      "<input type='text' name='info_values[]' class='span2' placeholder='Value (e.g. 1234)' value='"+value+"' " + cls + "> " +
      "<a href='#' class='link-danger delete-li' onclick=\"$(this).parent('li').remove();\"><i class='fa fa-times-circle'></i></a>";
  if (desc) {
    html += "<div class='entity-desc'>";
    if (link) {
      html += " <a href='" + link + "' title='more info' target='_blank'>"+ desc + " <i class='fa fa-zoom-in'></i></a> ";
    } else {
      html += desc;
    }

    html += "</div>";
  }
  html += "</li>";
  ul.prepend(html);
}

function recommendEntity(type, value, desc, link, forceUpdate) {
  addAdditionalInformation("annotation", type, value, desc, link, forceUpdate);
  $("#entityRecommenderModal").modal('hide');
  return;
}

function add_annotation(id, passage_offset, node_id, offset, length, type, desc, representative_name) {
  var e = $("#p_" + passage_offset);
  var text = e.html();
  //offset = offset +1;
  var from_pos = tpos2hpos(text, passage_offset, e.text().length, offset) - 1;
  var to_pos = tpos2hpos(text, passage_offset, e.text().length, offset + length);
  var h_offset = tpos2hpos(text, passage_offset, e.text().length, offset);
  var h_length = to_pos - from_pos - 1;
  var result_text = "";
  var pre_text = "", post_text = "";
  var atext = text.substr(from_pos + 1, h_length);
  // console.log(offset + ":" + length);
  // console.log(h_offset);
  // console.log(h_length);
  // console.log(atext);

  if (h_offset > 0)
    pre_text = text.substr(0, h_offset);

  post_text = text.substr(h_offset + h_length);

  if (!type) {
    type = "N/A";
  }

  var title = node_id + ":" + type;
  if (desc) {
    title += ":" + desc;
  }
  result_text = pre_text + "<span class='annotation a_" + type +
    "' data-id='"+id+"' data-type='" + type + "' " +
    " data-representative='"+representative_name+ "' " +
    " data-toggle='tooltip' title='" +  title + "' " +
    " data-a-id='" + node_id + "' id = 'a_" + node_id + "' data-offset='" +
    offset + "' data-length='" + length + "' >" + atext + "</span>" + post_text;

  e.html(result_text);
}

function getSelected() {
  if(window.getSelection) { return window.getSelection(); }
  else if(document.getSelection) { return document.getSelection(); }
  else {
    var selection = document.selection && document.selection.createRange();
    if(selection.text) { return selection.text; }
    return false;
  }
  return false;
}

function getPreviousOffset(range, passage) {
  var p = range.startContainer;
  var offset = $(passage).data("offset");
  var text;
  while(p.previousSibling) {
    p = p.previousSibling;
    text = (p.innerHTML) ? p.innerHTML : ((p.textContent) ? p.textContent:"");
    offset += text.length;
  }
  return offset;
}
function toggleRepresentativeNameEdit(val) {
  if (val === false) {
    $("#annotationModal input#representative_name").val("");
    $("#annotationModal .representative_name_edit").hide();
    $("#annotationModal .btn_representative_name_edit").show();
  } else {
    $("#annotationModal input#representative_name").val(val);
    $("#annotationModal .representative_name_edit").show();
    $("#annotationModal .btn_representative_name_edit").hide();
  }
}
function showAnnotation(id, text, representative_name, aid, type, key, offset, length, create, recommend) {
  console.log("Call! ShowAnnotation" + id + text + aid + type);
  if (recommend) {
    $("#annotationModal input#aid").val(aid);
    $("#annotationModal input#type").val(type);
    $("#annotationModal input#text").val(recommend.text);
    $("#annotationModal fieldset.find-all-div span.text").text(recommend.text);
    $("#annotationModal fieldset.find-all-div input").prop("checked", false);
    $("#annotationModal input#offset").val(recommend.offset);
    $("#annotationModal input#length").val(recommend.length);
  } else {
    $("#annotationModal input#text").val(text);
    if (representative_name && representative_name.trim().length > 0) {
      toggleRepresentativeNameEdit(representative_name.trim());
    } else {
      toggleRepresentativeNameEdit(false);
    }
    $("#annotationModal input#aid").val(aid);
    $("#annotationModal select#type").val(type);
    $("#annotationModal input#offset").val(offset);
    $("#annotationModal input#length").val(length);
    $("#annotationModal input#key").val(key);
  }

  $("#annotationModal ul.additional-information").empty();
  $("#annotationModal ul.annotation-locations").empty();
  addAdditionalInformation("annotation", "", "", null);
  $("#annotationModal .recommends").hide();
  $("#annotationModal .annotation-locations-div").hide();
  $("#annotationModal fieldset.find-all-div").hide();

  if (create) {
    $("#annotationModal h3").text("Create Annotation");
    $("#annotationModal .modal-footer .btn-danger").hide();
    $("#annotationModal .modal-footer .btn-primary").text("Create");
    $("#annotationModal .modal-footer .closeBtn").html("<i class='fa fa-chevron-left'></i> Back".html_safe);
    var found = false;
    $(".annotation_tr").each(function() {
      var e = $(this);
      var text = e.find("span.text").text();
      if (text.toUpperCase() === recommend.text.toUpperCase()) {
        found = true;
        $("#annotationModal .recommends table tbody").append(
          "<tr>" +
          "<td>" + e.find("td:nth-child(1)").html() + "</td>" +
          "<td>" + e.find("td:nth-child(2)").html() + "</td>" +
          "<td>" + e.find("td:nth-child(3)").html() + "</td>" +
          "<td>" + e.find("td:nth-child(4)").html() + "</td>" +
          "<td><a href='/nodes/" + e.data('id') + "/merge?offset=" + recommend.offset +
          "' class='btn btn-mini btn-danger'> <i class='fa fa-plus-sign'></i> Merge</a></td>" +
          "<tr>"
        );
      }
    });
    if (found) {
      $("#annotationModal .recommends").show();
    }

    $("#annotationModal fieldset.find-all-div").show();
    $("#annotationModal .find-all-div input").prop("checked", false);
  }else {
    var cnt = 0;
    var cls;
    var html;
    $("#annotationModal ul.annotation-locations").empty();
    $("div.passage span.annotation#a_" + aid).each(function() {
      var e = $(this);
      var tooltip = "";
      cnt += 1;

      if (offset && offset == e.data('offset')) {
        tooltip = " class ='loc-tooltip' style='background:#ffd;' title='just clicked location' ";
      }

      html = "<li " + tooltip + ">";
      if (offset && offset == e.data('offset')) {
        html += "<i class='fa fa-check'></i> ";
      }
      html += "<span class='loc'>" + e.data('offset') + ":" + e.data('length');
      html += "</span>" +
        " <a class='link-danger' data-confirm='Are you sure?' href='/nodes/" + e.data('id') + "/remove?offset=" + e.data('offset') + "'>" +
        "<i class='fa fa-remove'></i></a>";

      html += "</li>";
      $("#annotationModal ul.annotation-locations").append(html);
    });

    $("#annotationModal .add-all").attr("href", "/nodes/" + id + "/add_all");

    $("#annotationModal .annotation-locations-div").show();
    if (cnt < 2) {
     $("#annotationModal .annotation-locations a").hide();
    }

    $("#annotationModal h3").text("Edit Annotation");
    setDeleteAnnotationUrl(id);
    $("#annotationModal .modal-footer .btn-danger").show();
    $("#annotationModal .modal-footer .btn-primary").text("Update");
    $("#annotationModal .modal-footer .closeBtn").html("Close");

    $("#annotationModal form").data("id", id);
    $("#annotationModal .hide-for-update").hide();
    $("#locationSelector").modal("hide");
    $.getJSON('/nodes/'+id+'/info.json', function(data) {
      for(var i = 0; i < data.length; i++) {
        var item = data[i];
        addAdditionalInformation("annotation", item.key, item.value, item.desc, item.link);
      }
    });

    $(".loc-tooltip").tooltip();
  }

  clearSelection();

  $("#annotationModal .btn-primary").prop("disabled", false).removeClass("ui-state-disabled");
  $('#annotationModal').modal({backdrop: 'static', show: true});
}

function clearSelection() {
  if (window.getSelection) {
    if (window.getSelection().empty) {  // Chrome
      window.getSelection().empty();
    } else if (window.getSelection().removeAllRanges) {  // Firefox
      window.getSelection().removeAllRanges();
    }
  } else if (document.selection) {  // IE?
    document.selection.empty();
  }
}
function addRefInRelation(id, type, text, label) {
  var e = $("<li data-id='"+id+"' data-type='"+type+"'></li>");
  e.append("<i class='fa fa-move'></i>");
  e.append("<input class='span1' style='width:40px;' type='text' readonly name='refids[]' value='"+id+"'> ");
  e.append("<input type='hidden' name='reftypes[]' value='"+type+"'>");
  e.append("<input class='span2' type='text' readonly name='text[]' value='"+text+"'> ");
  e.append("<input class='span2' style='width:200px;' type='text' name='labels[]' value='"+label+"' placeholder='Describe role if necessary'>");
  e.append(" <a href='#' class='link-danger' onclick='removeRefInRelation($(this).parent());return false;'><i class='fa fa-times-circle'></i></a>");

  e.appendTo("#relationModal ol");
}

function removeRefInRelation(e) {
  if (confirm("Are you sure?")) {
    e.remove();
  }
}

function showRelation(id, rid, type, refs, create) {
  $("#relationModal input#rid").val(rid);
  $("#relationModal select#type").val(type);
  $("#relationModal ol").html("");

  if (refs) {
    refs.each(function() {
      var e = $(this);
      addRefInRelation(e.data("id"), e.data("type"), e.data("text"), e.data("label"));
    });
  }

  $("#relationModal ul.additional-information").html("");
  addAdditionalInformation("relation", "", "", null);

  if (create) {
    $("#relationModal h3").text("Create Relation");
    $("#relationModal .modal-footer .btn-danger").hide();
    $("#relationModal .modal-footer .btn-primary").text("Create");
  }else {
    $("#relationModal h3").text("Edit Relation");
    setDeleteRelationUrl(id);
    $("#relationModal .modal-footer .btn-danger").show();
    $("#relationModal .modal-footer .btn-primary").text("Update");
    $("#relationModal form").data("id", id);
    $.getJSON('/nodes/'+id+'/info.json', function(data) {
      for(var i = 0; i < data.length; i++) {
        var item = data[i];
        addAdditionalInformation("relation", item.key, item.value, item.desc);
      }
    });
  }
  $("#relationModal ol").sortable({
    handle: 'i.fa-move'
  });
  $("#relationModal .btn-primary").prop("disabled", false).removeClass("ui-state-disabled");
  $("#relationModal").modal("show");
}

function uniquePush(ret, item) {
  for(var i in ret) {
    var obj = ret[i];
    if (obj.offset == item.offset && obj.length == item.length) {
      return;
    }
  }

  ret.push(item);
}

function getRecommendText(passageOffset, range) {
  var ret = [];

  var node = range.startContainer;
  var startPos = range.startOffset;
  var offset = passageOffset + startPos;
  var text = range.toString();
  var length = text.length;
  var ch;
  var nodeText = $(node).text();
  var recommendText, rec_offset, rec_length;
  var found = false;

  uniquePush(ret, {text: text, offset: offset, length: length});

  if ((range.startOffset > 0 && nodeText.charAt(startPos - 1) != ' ') ||
      (range.startOffset + length < nodeText.length && nodeText.charAt(startPos + length ) != ' '))
  {
    rec_offset = startPos;
    rec_length = length;

    while (rec_offset > 0 && nodeText.charAt(rec_offset - 1) != ' ') {
      rec_offset--;
      rec_length++;
    }

    while (rec_offset + rec_length < nodeText.length && nodeText.charAt(rec_offset + rec_length) != ' ') {
      rec_length++;
    }

    recommendText = nodeText.substr(rec_offset, rec_length);
    rec_offset += passageOffset;
    uniquePush(ret, {text: recommendText, offset: rec_offset, length: rec_length});


    found = false;

    // Trimming start whitespace
    while (rec_length > 0 && ".,()".indexOf(recommendText.charAt(0)) >= 0) {
      rec_offset++;
      rec_length--;
      found = true;

      recommendText = recommendText.substr(1);
    }

    // Trimming end whitespace
    while (rec_length > 0 && ".,()".indexOf(recommendText.charAt(rec_length - 1)) >= 0) {
      rec_length--;
      found = true;

      recommendText = recommendText.substr(0, rec_length);
    }

    if (found) {
      uniquePush(ret, {text: recommendText, offset: rec_offset, length: rec_length});
    }
  }

  rec_offset = offset;
  rec_length = length;
  recommendText = text;

  found = false;
  // Trimming start whitespace
  while (rec_length > 0 && (recommendText.charAt(0)) == ' ') {
    rec_offset++;
    rec_length--;
    recommendText = recommendText.substr(1);
    found = true;
  }

  // Trimming end whitespace
  while (rec_length > 0 && (recommendText.charAt(rec_length - 1)) == ' ') {
    rec_length--;
    recommendText = recommendText.substr(0, rec_length);
    found = true;
  }

  if (found) {
    uniquePush(ret, {text: recommendText, offset: rec_offset, length: rec_length});
  }

  found = false;

  // Trimming start whitespace
  while (rec_length > 0 && ".,()".indexOf(recommendText.charAt(0)) >= 0) {
    rec_offset++;
    rec_length--;
    found = true;

    recommendText = recommendText.substr(1);
  }

  // Trimming end whitespace
  while (rec_length > 0 && ".,()".indexOf(recommendText.charAt(rec_length - 1)) >= 0) {
    rec_length--;
    found = true;

    recommendText = recommendText.substr(0, rec_length);
  }

  if (found) {
    uniquePush(ret, {text: recommendText, offset: rec_offset, length: rec_length});
  }
  return ret;
}

function showLocationSelector(list, range, passageOffset) {
  var minPos = 999999;
  var maxPos = -99999;
  var node = range.startContainer;
  var nodeText = $(node).text();

  $("#locationSelector table.recommends tbody").empty();

  _.each(list, function(obj) {
    if (obj.length <= 0) return;
    if (obj.offset < minPos) {
      minPos = obj.offset;
    }
    if (obj.offset + obj.length > maxPos) {
      maxPos = obj.offset + obj.length;
    }

    var trHtml = "<tr data-offset='" + obj.offset + "' data-length='" + obj.length + "'>" +
                      "<td>'" + obj.text + "'</td><td>" + obj.offset + "</td>" + "<td>" + obj.length + "</td>" +
                      "<td><button class='btn btn-mini'>Change Range</button></td></tr>";
    $("#locationSelector table.recommends tbody").prepend(trHtml);
  });

  var trCnt = _.size($("#locationSelector table.recommends tbody tr"));
  if (trCnt == 0 && list[0].length === 0) {
    minPos = list[0].offset;
    maxPos = list[0].offset + 1;
  }

  $("#locationSelector table.recommends tbody tr button").click(function(e) {
    var tr = $(e.target).parents("tr");
    var selection = [tr.data("offset"), tr.data("offset") + tr.data("length") - 1];
    console.log('chage -> ' + selection);
    changeSliderSelection(selection);
  });

  minPos = minPos - 30;
  maxPos = maxPos + 30;

  if (minPos < passageOffset) {
    minPos = passageOffset;
  }

  if (maxPos > nodeText.length + passageOffset) {
    maxPos = nodeText.length + passageOffset;
  }

  var text = nodeText.substr(minPos - passageOffset, maxPos - minPos + 1);
  var offset = minPos;
  var startPos = range.startOffset + passageOffset;
  var endPos = startPos + range.toString().length - 1;
  if (endPos < startPos) {
    endPos = startPos;
  }
  console.log(range);
  console.log("MIN:MAX" + offset + ":" + (offset+text.length - 1));
  var selection = [startPos, endPos];
  $("#locationSelector .slider-div .start").text(offset);
  $("#locationSelector .slider-div .end").text(offset + text.length - 1);
  $("#locationSelector .text").data("offset", offset);
  $("#locationSelector .text").text(text);
  $("#locationSelector .text-real").text(text);
  console.log(selection);
  $("#rangeSliderDiv").html("<div id='rangeSlider'></div>");
  $("#rangeSlider").slider({tooltip:'hide', min: offset, max: offset + text.length - 1, value: selection}).on('slide', function(ev){
    markSelection(ev.value);
  });
  console.log("RANGE: [" + range.toString() + "]");
  if (range.toString().length === 0 && trCnt > 0) {
    $("#locationSelector table.recommends tbody tr button:first").click();
  } else {
    changeSliderSelection(selection);
  }

  clearSelection();

  $("#locationSelector").modal({backdrop: 'static', show: true});
}

function changeSliderSelection(selection) {
  var s = selection[0];
  var e = selection[1];

  console.log("selection1 : " + [s, e]);
  $("#rangeSlider").slider('setValue', [s, e]);
  console.log("selection2 : " + [s, e]);
  markSelection([s, e]);
}

function markSelection(selection) {
  var min = $("#locationSelector .text").data("offset");
  var text = $("#locationSelector .text").text();
  text = text.replace(/&nbsp;/g , " ");
  var maxLength = text.length;
  var offset = selection[0];
  var length = selection[1] - selection[0] + 1;

  if (offset < min) {
    offset = min;
  }

  if (offset + length > min + maxLength) {
    if (offset >= min + maxLength) {
      offset = min + maxLength - 1;
    }
    if (offset + length > min + maxLength) {
      length = min + maxLength - offset;
    }
  }
  console.log("selection in markSelection : " + selection);
  console.log(offset + ":" + length);
  var part1 = text.substr(0, offset - min);
  var part2 = text.substr(offset - min, length);
  var part3 = text.substr(offset - min + length, maxLength - (offset - min + length));
  console.log("orig:'" + text+"'");
  console.log("1:'" + part1+"'");
  console.log("2:'" + part2+"'");
  console.log("3:'" + part3+"'");
  text = part1 + "<span>" + part2 + "</span>" + part3;
  text = text.replace(/ /g , "&nbsp;");
  $("#locationSelector .text").html(text);
  $("#locationSelector #loc_offset").val(offset);
  $("#locationSelector #loc_length").val(length);
  return [offset, offset + length - 1];
}


